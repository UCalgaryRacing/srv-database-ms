package middleware

import (
	"context"
	"database-ms/app/model"
	services "database-ms/app/services"
	"database-ms/app/utils"
	"database-ms/config"
	"errors"
	"fmt"
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v4"
	"github.com/google/uuid"
	"gorm.io/gorm"
)

var Roles = map[string]int{
	"Admin":   4,
	"Lead":    3,
	"Member":  2,
	"Guest":   1,
	"Pending": 0,
}

// Returns the organization associated with the authorization
func AuthorizationMiddleware(conf *config.Configuration, db *gorm.DB) gin.HandlerFunc {
	organizationService := services.NewOrganizationService(db, conf)
	userService := services.NewUserService(db, conf)

	return func(ctx *gin.Context) {
		// Initialize admin flags to false.
		ctx.Set("super-admin", false)
		ctx.Set("org-admin", false)

		// Check API Key
		apiKey := ctx.Request.Header.Get("apiKey")

		// Check if API Key is the admin secret.
		switch apiKey {
		case "":
			break
		case conf.AdminKey:
			ctx.Set("super-admin", true)
			ctx.Next()
			return
		default:
			// Check if Api Key matches an organization.
			organization, err := organizationService.FindByOrganizationApiKey(context.TODO(), apiKey)
			if err != nil {
				utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
				return
			}

			// If an org is found, grant admin permissions on that org.
			ctx.Set("organization", organization)
			ctx.Set("org-admin", true)
			ctx.Next()
			return
		}

		// Check JWT token
		tokenString, err := ctx.Cookie("Authorization")
		if tokenString == "" || err != nil {
			utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
			ctx.Abort()
			return
		}

		// Parse the token
		var hmacSampleSecret = []byte(conf.AccessSecret)
		token, err := jwt.Parse(tokenString, func(t *jwt.Token) (interface{}, error) {
			if _, ok := t.Method.(*jwt.SigningMethodHMAC); !ok {
				return nil, fmt.Errorf("unexpected signing method: %v", t.Header["alg"])
			}
			return hmacSampleSecret, nil
		})

		// Check if token is blacklisted
		if userService.IsBlacklisted(token) {
			utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.InvalidatedToken))
			ctx.Abort()
			return
		}

		// Handle invalid token
		if !token.Valid {
			if errors.Is(err, jwt.ErrTokenMalformed) {
				utils.Response(ctx, http.StatusBadRequest, utils.NewHTTPError(utils.MalformedToken))
			} else if errors.Is(err, jwt.ErrTokenExpired) {
				utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.ExpiredToken))
			} else {
				utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.InvalidToken))
			}
			ctx.Abort()
			return
		}

		// Decode the payload
		if claims, ok := token.Claims.(jwt.MapClaims); ok && token.Valid {
			// Extract the user id
			userId, err := uuid.Parse(fmt.Sprintf("%s", claims["userId"]))
			if err != nil {
				utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
				ctx.Abort()
				return
			}

			// Load the user data
			user, perr := userService.FindByUserId(context.TODO(), userId)
			if perr != nil {
				utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
				ctx.Abort()
				return
			}

			// Load the users organization data
			organization, perr := organizationService.FindByOrganizationId(context.TODO(), user.OrganizationId)
			if perr != nil {
				utils.Response(ctx, http.StatusUnauthorized, utils.NewHTTPError(utils.Unauthorized))
				ctx.Abort()
				return
			}
			ctx.Set("user", user)
			ctx.Set("organization", organization)
			ctx.Set("token", token)
		} else {
			utils.Response(ctx, http.StatusInternalServerError, utils.NewHTTPError(utils.InternalError))
			ctx.Abort()
		}
	}
}

func GetOrganizationClaim(ctx *gin.Context) (*model.Organization, error) {
	organizationInterface, organizationExists := ctx.Get("organization")
	if organizationExists {
		return organizationInterface.(*model.Organization), nil
	} else {
		fmt.Println("No organization claim found")
		return nil, gin.Error{}
	}
}

func GetUserClaim(ctx *gin.Context) (*model.User, error) {
	userInterface, userExists := ctx.Get("user")
	if userExists {
		return userInterface.(*model.User), nil
	} else {
		return nil, gin.Error{}
	}
}

func GetToken(ctx *gin.Context) (*jwt.Token, error) {
	token, tokenExists := ctx.Get("token")
	if !tokenExists {
		return &jwt.Token{}, gin.Error{}
	}
	return token.(*jwt.Token), nil
}

func IsAuthorizationAtLeast(ctx *gin.Context, role string) bool {
	user, err := GetUserClaim(ctx)
	if err != nil {
		return ctx.GetBool("org-admin")
	} else {
		return Roles[user.Role] >= Roles[role]
	}
}

func IsSuperAdmin(ctx *gin.Context) bool {
	return ctx.GetBool("super-admin")
}
