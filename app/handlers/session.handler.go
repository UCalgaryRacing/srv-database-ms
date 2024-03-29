package handlers

import (
	"bytes"
	"database-ms/app/middleware"
	"database-ms/app/model"
	services "database-ms/app/services"
	utils "database-ms/app/utils"
	"io"
	"net/http"
	"os"
	"path/filepath"

	"github.com/gin-gonic/gin"
	"github.com/google/uuid"
)

type SessionHandler struct {
	session  services.SessionServiceInterface
	thing    services.ThingServiceInterface
	filepath string
}

func NewSessionAPI(
	sessionService services.SessionServiceInterface,
	thingService services.ThingServiceInterface,
	filepath string,
) *SessionHandler {
	return &SessionHandler{
		session:  sessionService,
		thing:    thingService,
		filepath: filepath,
	}
}

func (handler *SessionHandler) CreateSession(ctx *gin.Context) {
	// Guard against non-lead+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to parse the body
	var newSession model.Session
	err := ctx.BindJSON(&newSession)
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.BadRequest))
		return
	}

	// Guard against non-lead+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to find the thing
	thing, perr := handler.thing.FindById(ctx, newSession.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant writes
	organization, _ := middleware.GetOrganizationClaim(ctx)
	if organization.Id != thing.OrganizationId {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to create the session
	notGenerated := false
	newSession.Generated = &notGenerated
	perr = handler.session.CreateSession(ctx.Request.Context(), &newSession)
	if perr != nil {
		if perr.Code == "23505" {
			utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.SessionNotUnique))
		} else {
			utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.EntityCreationError))
		}
		return
	}

	// Send the response
	result := utils.SuccessPayload(newSession, "Successfully created collection.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *SessionHandler) GetSessions(ctx *gin.Context) {
	// Attempt to read from the params
	thingId, err := uuid.Parse(ctx.Param("thingId"))
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		return
	}

	// Attempt to find the thing
	thing, perr := handler.thing.FindById(ctx.Request.Context(), thingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant reading
	organization, _ := middleware.GetOrganizationClaim(ctx)
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read the sessions
	sessions, perr := handler.session.FindSessionsByThingId(ctx.Request.Context(), thingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.SessionsNotFound))
		return
	}

	// Attempt the file sizes to each session
	for i := range sessions {
		file := handler.filepath + thingId.String() + "/" + sessions[i].Name + ".csv"
		fi, err := os.Stat(file)
		if err == nil {
			sessions[i].FileSize = fi.Size()
		}
	}

	// Send the response
	result := utils.SuccessPayload(sessions, "Successfully retrieved sessions.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *SessionHandler) UpdateSession(ctx *gin.Context) {
	// Guard against non-lead+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to extract the body
	var updatedSession model.Session
	err := ctx.BindJSON(&updatedSession)
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.BadRequest))
		return
	}

	// Guard against non-lead+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to get the thing
	thing, perr := handler.thing.FindById(ctx, updatedSession.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant updates
	organization, _ := middleware.GetOrganizationClaim(ctx)
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Read the current session and don't allow updates to the thingId or generated field
	session, perr := handler.session.FindById(ctx.Request.Context(), updatedSession.Id)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.SessionNotFound))
		return
	}
	updatedSession.ThingId = session.ThingId
	updatedSession.Generated = session.Generated

	// Attempt to rename the file on the file system if the session name changed
	if session.Name != updatedSession.Name && session.EndTime != nil {
		err = os.Rename(
			handler.filepath+session.ThingId.String()+"/"+session.Name+".csv",
			handler.filepath+session.ThingId.String()+"/"+updatedSession.Name+".csv",
		)
		// if err != nil {
		// 	utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPError(utils.FailedToRenameFile))
		// 	return
		// }
	}

	// Attempt to update the collection
	perr = handler.session.UpdateSession(ctx.Request.Context(), &updatedSession)
	if perr != nil {
		if perr.Code == "23505" {
			utils.Response(ctx, http.StatusConflict, utils.NewHTTPError(perr.Error()))
		} else {
			utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		}
		return
	}

	// Send the response
	result := utils.SuccessPayload(nil, "Successfully updated.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *SessionHandler) DeleteSession(ctx *gin.Context) {
	// Guard against non-lead+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read from the params
	sessionId, err := uuid.Parse(ctx.Param("sessionId"))
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		return
	}

	// Attempt to find the session
	session, perr := handler.session.FindById(ctx.Request.Context(), sessionId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.SessionNotFound))
		return
	}

	// Attempt to find the thing
	thing, perr := handler.thing.FindById(ctx, session.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant deletion
	organization, _ := middleware.GetOrganizationClaim(ctx)
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to delete session file
	if err = os.Remove(handler.filepath + session.ThingId.String() + "/" + session.Name + ".csv"); err != nil {
		// utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPError(utils.FailedToDeleteFile))
		// return
	}

	// Attempt to delete the session
	perr = handler.session.DeleteSession(ctx.Request.Context(), sessionId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, perr.Error()))
		return
	}

	// Send the response
	result := utils.SuccessPayload(nil, "Successfully deleted")
	utils.Response(ctx, http.StatusOK, result)
}

// TODO: Attempt to insert data
func (handler *SessionHandler) UploadFile(ctx *gin.Context) {
	// Guard against non-lead+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read from the params
	sessionId, err := uuid.Parse(ctx.Param("sessionId"))
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		return
	}

	// Attempt to get the session
	session, perr := handler.session.FindById(ctx, sessionId)
	if perr != nil {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.SessionNotFound))
		return
	}

	// Attempt to get the associated thing
	thing, perr := handler.thing.FindById(ctx, session.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant uploads
	organization, _ := middleware.GetOrganizationClaim(ctx)
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read the file
	file, err := ctx.FormFile("file")
	if err != nil {
		result := utils.NewHTTPError(utils.NoFileRcvd)
		utils.Response(ctx, http.StatusBadRequest, result)
		return
	}

	// Verify file extension (.csv)
	if extension := filepath.Ext(file.Filename); extension != ".csv" {
		result := utils.NewHTTPError(utils.NotCsv)
		utils.Response(ctx, http.StatusBadRequest, result)
		return
	}

	// Attempt to the directory
	err = os.MkdirAll(handler.filepath+session.ThingId.String(), 0777)
	if err != nil && !os.IsExist(err) {
		utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPError(err.Error()))
		return
	}

	// Attempt to save the file
	if err = ctx.SaveUploadedFile(file, handler.filepath+session.ThingId.String()+"/"+session.Name+".csv"); err != nil {
		utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPError(utils.CouldNotUploadFile))
		return
	}

	// Send the response
	result := utils.SuccessPayload(nil, "Successfully uploaded file")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *SessionHandler) DownloadFile(ctx *gin.Context) {
	// Guard against non-member+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Member") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read from the params
	sessionId, err := uuid.Parse(ctx.Param("sessionId"))
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		return
	}

	// Attempt to read the session
	session, perr := handler.session.FindById(ctx, sessionId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.SessionNotFound))
		return
	}

	// Attempt to read the thing
	thing, perr := handler.thing.FindById(ctx, session.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant reads
	organization, _ := middleware.GetOrganizationClaim(ctx)
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read the file
	file, err := os.Open(handler.filepath + session.ThingId.String() + "/" + session.Name + ".csv")
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.FileNotFound))
		return
	}
	defer file.Close()

	// Attempt to place the data into a buffer
	buf := &bytes.Buffer{}
	nRead, err := io.Copy(buf, file)
	if err != nil {
		utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPError(err.Error()))
		return
	}

	// Send the response
	ctx.DataFromReader(http.StatusOK, nRead, "text/csv", buf, nil)
}
