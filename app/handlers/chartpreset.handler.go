package handlers

import (
	"database-ms/app/middleware"
	"database-ms/app/model"
	"database-ms/app/services"
	"database-ms/app/utils"
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/google/uuid"
)

type ChartPresetHandler struct {
	service      services.ChartPresetServiceInterface
	thingService services.ThingServiceInterface
}

func NewChartPresetAPI(service services.ChartPresetServiceInterface, thingService services.ThingServiceInterface) *ChartPresetHandler {
	return &ChartPresetHandler{service: service, thingService: thingService}
}

func (handler *ChartPresetHandler) CreateChartPreset(ctx *gin.Context) {
	// Guard against non-member+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Member") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to extract the body
	var newChartPreset model.ChartPreset
	err := ctx.BindJSON(&newChartPreset)
	if len(newChartPreset.Charts) == 0 || err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ChartPresetNotValid))
		return
	}

	// Attempt to find the associated thing
	organization, _ := middleware.GetOrganizationClaim(ctx)
	thing, perr := handler.thingService.FindById(ctx, newChartPreset.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant writing
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Ensure the preset is valid
	perr = handler.service.Create(ctx, &newChartPreset)
	if perr != nil {
		if perr.Code == "23505" {
			utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ChartPresetNotUnique))
		} else {
			utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPCustomError(utils.InternalError, perr.Error()))
		}
		return
	}

	// Send the response
	result := utils.SuccessPayload(newChartPreset, "Successfully created Chart Preset.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *ChartPresetHandler) GetChartPresets(ctx *gin.Context) {
	// Attempt to parse the query param
	thingId, err := uuid.Parse(ctx.Param("thingId"))
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		return
	}

	// Attempt to find the associated thing
	organization, _ := middleware.GetOrganizationClaim(ctx)
	thing, perr := handler.thingService.FindById(ctx, thingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant reading
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read the presets
	chartPresets, perr := handler.service.FindByThingId(ctx, thingId)
	if perr != nil {
		utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPCustomError(utils.InternalError, perr.Error()))
		return
	}

	// Send the response
	result := utils.SuccessPayload(chartPresets, "Successfully retrieved Chart Presets.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *ChartPresetHandler) UpdateChartPreset(ctx *gin.Context) {
	// Guard against non-member+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Member") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to extract the body
	var updatedChartPreset model.ChartPreset
	err := ctx.BindJSON(&updatedChartPreset)
	if len(updatedChartPreset.Charts) == 0 || err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ChartPresetNotValid))
		return
	}

	// Attempt to find the associated thing
	organization, _ := middleware.GetOrganizationClaim(ctx)
	thing, perr := handler.thingService.FindById(ctx, updatedChartPreset.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant updates
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to update the preset
	perr = handler.service.Update(ctx, &updatedChartPreset)
	if perr != nil {
		if perr.Code == "23505" {
			utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ChartPresetNotUnique))
		} else {
			utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPCustomError(utils.InternalError, perr.Error()))
		}
	}

	// Send the response
	result := utils.SuccessPayload(updatedChartPreset, "Successfully Updated.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *ChartPresetHandler) DeleteChartPreset(ctx *gin.Context) {
	// Guard against non-member+ requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Member") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to read the params
	chartPresetId, err := uuid.Parse(ctx.Param("chartPresetId"))
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		return
	}

	// Attempt to find the existing chart prest
	organization, _ := middleware.GetOrganizationClaim(ctx)
	chartPreset, perr := handler.service.FindById(ctx, chartPresetId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ChartPresetNotFound))
		return
	}

	// Attempt to find the associated thing
	thing, perr := handler.thingService.FindById(ctx, chartPreset.ThingId)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.ThingNotFound))
		return
	}

	// Guard against cross-tenant deletion
	if thing.OrganizationId != organization.Id {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to delete the chart preset
	perr = handler.service.Delete(ctx, chartPresetId)
	if perr != nil {
		utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPCustomError(utils.InternalError, perr.Error()))
		return
	}

	// Send the response
	result := utils.SuccessPayload(nil, "Successfully deleted.")
	utils.Response(ctx, http.StatusOK, result)
}
