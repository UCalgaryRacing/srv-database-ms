package handlers

import (
	"database-ms/app/middleware"
	"database-ms/app/model"
	services "database-ms/app/services"
	utils "database-ms/app/utils"
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/google/uuid"
)

type OperatorHandler struct {
	service services.OperatorServiceInterface
}

func NewOperatorAPI(operatorService services.OperatorServiceInterface) *OperatorHandler {
	return &OperatorHandler{service: operatorService}
}

func (handler *OperatorHandler) CreateOperator(ctx *gin.Context) {
	// Guard against non-admin users
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to extract the body
	var newOperator model.Operator
	err := ctx.BindJSON(&newOperator)
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.BadRequest))
		return
	}

	// Attempt to create the operator
	organization, _ := middleware.GetOrganizationClaim(ctx)
	newOperator.OrganizationId = organization.Id
	perr := handler.service.Create(ctx.Request.Context(), &newOperator)
	if perr != nil {
		if perr.Code == "23505" {
			utils.Response(ctx, http.StatusConflict, utils.NewHTTPError(utils.OperatorNotUnique))
		} else {
			utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.EntityCreationError))
		}
		return
	}

	// Send the response
	result := utils.SuccessPayload(newOperator, "Successfully created operator.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *OperatorHandler) GetOperators(ctx *gin.Context) {
	// Attempt to get the operators
	organization, _ := middleware.GetOrganizationClaim(ctx)
	operators, perr := handler.service.FindByOrganizationId(ctx.Request.Context(), organization.Id)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.BadRequest))
		return
	}

	// Send the response
	result := utils.SuccessPayload(operators, "Successfully retrieved operators.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *OperatorHandler) UpdateOperator(ctx *gin.Context) {
	// Guard against non-admin users
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to extract the body
	var updatedOperator model.Operator
	err := ctx.BindJSON(&updatedOperator)
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.BadRequest))
		return
	}

	// Attempt to find the existing operator
	organization, _ := middleware.GetOrganizationClaim(ctx)
	operator, perr := handler.service.FindById(ctx, updatedOperator.Id)
	if perr != nil {
		utils.Response(ctx, http.StatusNotFound, utils.NewHTTPError(utils.OperatorNotFound))
		return
	}

	// Guard against cross-tenant writing
	if organization.Id != operator.OrganizationId {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to update the operator
	updatedOperator.OrganizationId = organization.Id
	perr = handler.service.Update(ctx.Request.Context(), &updatedOperator)
	if perr != nil {
		if perr.Code == "23505" {
			utils.Response(ctx, http.StatusConflict, utils.NewHTTPError(utils.OperatorNotUnique))
		} else {
			utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.EntityCreationError))
		}
		return
	}

	// Send the response
	result := utils.SuccessPayload(nil, "Successfully updated operator.")
	utils.Response(ctx, http.StatusOK, result)
}

func (handler *OperatorHandler) DeleteOperator(ctx *gin.Context) {
	// Guard against non-admin requests
	if !middleware.IsAuthorizationAtLeast(ctx, "Lead") {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.OperatorNotFound))
		return
	}

	// Attempt to parse the query param
	organization, _ := middleware.GetOrganizationClaim(ctx)
	operatorIdToDelete, err := uuid.Parse(ctx.Param("operatorId"))
	if err != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPCustomError(utils.BadRequest, err.Error()))
		return
	}

	// Attempt to find the operator to delete
	operator, perr := handler.service.FindById(ctx, operatorIdToDelete)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.BadRequest))
		return
	}

	// Guard against cross-tenant deletions
	if organization.Id != operator.OrganizationId {
		utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
		return
	}

	// Attempt to delete the operator
	perr = handler.service.Delete(ctx.Request.Context(), operator.Id)
	if perr != nil {
		utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.BadRequest))
		return
	}

	// Send the response
	result := utils.SuccessPayload(nil, "Successfully deleted operator.")
	utils.Response(ctx, http.StatusOK, result)
}
