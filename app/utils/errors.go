package utils

import (
	"errors"

	"github.com/jackc/pgconn"
)

func NewHTTPError(errorCode string) map[string]interface{} {
	m := make(map[string]interface{})
	m["error"] = errorCode
	m["error_description"] = errorMessage[errorCode]
	return m
}

func NewHTTPCustomError(errorCode, errorMsg string) map[string]interface{} {
	m := make(map[string]interface{})
	m["error"] = errorCode
	m["error_description"] = errorMsg
	return m
}

func GetPostgresError(err error) *pgconn.PgError {
	var perr *pgconn.PgError
	errors.As(err, &perr)
	return perr
}

// Error codes - This can be done more nicely
const (
	// Other errors
	InternalError       = "internalError"
	InvalidBindingModel = "invalidBindingModel"
	EntityCreationError = "entityCreationError"
	BadRequest          = "badRequest"
	Unauthorized        = "unauthorized"
	Forbidden           = "forbidden"

	// Sensor errors
	SensorsNotFound     = "sensorsNotFound"
	SensorNotFound      = "sensorNotFound"
	SensorAlreadyExists = "sensorAlreadyExists"
	SensorNotUnique     = "sensorNotUnique"

	// User error
	UserNotFound    = "userNotFound"
	UsersNotFound   = "usersNotFound"
	WrongPassword   = "wrongPassword"
	UserNotApproved = "userPendingApproval"
	UserConflict    = "userConflict"
	UserLastAdmin   = "userLastAdmin"

	// Thing error
	ThingsNotFound = "thingsNotFound"
	ThingNotFound  = "thingNotFound"
	ThingNotUnique = "thingNotUnique"

	// Operator error
	OperatorsNotFound = "operatorsNotFound"
	OperatorNotFound  = "operatorNotFound"
	OperatorNotUnique = "operatorNotUnique"

	// Organization error
	OrganizationDuplicate = "organizationDuplicate"
	OrganizationNotFound  = "organizationNotFound"
	OrganizationsNotFound = "organizationsNotFound"

	// Raw Data Preset Error
	RawDataPresetNotUnique = "rawDataPresetNotUnique"
	RawDataPresetNotValid  = "rawDataPresetNotValid"
	RawDataPresetNotFound  = "rawDataPresetNotFound"

	// Chart Preset Error
	ChartPresetNotUnique = "chartPresetNotUnique"
	ChartPresetNotValid  = "chartPresetNotValid"
	ChartPresetNotFound  = "chartPresetNotFound"

	// Datum Error
	DatumNotFound = "datumNotFound"

	// Authorization Error
	MalformedToken   = "malformedToken"
	ExpiredToken     = "expiredToken"
	InvalidatedToken = "invalidatedToken"
	InvalidToken     = "invalidToken"
	EmptyToken       = "emptyToken"

	// Collection Error
	CollectionsNotFound = "collectionsNotFound"
	CollectionNotFound  = "collectionNotFound"
	CollectionNotUnique = "collectionNotUnique"

	// Session Error
	SessionsNotFound = "sessionsNotFound"
	SessionNotFound  = "sessionNotFound"
	SessionNotUnique = "sessionNotUnique"

	// Comments Error
	CommentsNotFound       = "commentsNotFound"
	CommentNotFound        = "commentNotFound"
	CommentContextNotFound = "commentContextNotFound"

	// File Error
	NotCsv                 = "notCSV"
	NoFileRcvd             = "noFileRcvd"
	CouldNotUploadFile     = "couldNotUploadFile"
	FileNotFound           = "fileNotFound"
	SubFolderCreationError = "subFolderCreationError"
	FailedToDeleteFile     = "failedToDeleteFile"
	FailedToDeleteFiles    = "failedToDeleteFiles"
	FailedToRenameFile     = "failedToRenameFile"
)

// Error code with description
var errorMessage = map[string]string{
	// Generic errors
	"internalError":       "An internal error occurred.",
	"invalidBindingModel": "The model could not be bound.",
	"entityCreationError": "Could not create entity.",
	"unauthorized":        "Unauthorized.",
	"forbidden":           "Forbidden.",

	// Sensor errors
	"sensorAlreadyExists": "Sensor already exists.",
	"sensorsNotFound":     "Sensors could not be found.",
	"sensorNotFound":      "Sensor could not be found.",
	"sensorNotUnique":     "Sensor name and CAN ID must be unique for a thing.",

	// User errors
	"userNotFound":  "User could not be found.",
	"usersNotFound": "Users not found.",
	"wrongPassword": "Password was incorrect.",
	"userConflict":  "Email must be globally unique and name must be organizationally unique.",
	"userLastAdmin": "The last administrator in the organization cannot be deleted or have their role changed.",

	// Thing
	"thingsNotFound": "Things could not be found.",
	"thingNotFound":  "Thing could not be found.",
	"thingNotUnique": "Thing name must be unique",

	// Operator
	"operatorsNotFound": "Operators could not be found.",
	"operatorNotFound":  "Operator could not be found.",
	"operatorNotUnique": "Operator name must be unique.",

	// Organization
	"organizationDuplicate": "Organization name is taken.",
	"organizationNotFound":  "Organization could not be found.",
	"organizationsNotFound": "Organizations could not be found.",

	// File
	"noFileReceived":         "No file is received.",
	"notCsv":                 "Not a csv.",
	"runHasAssociatedFile":   "Run already has associated file.",
	"runHasNoAssociatedFile": "Run does exist or not have associated file.",
	"cannotRetrieveFile":     "Cannot retrieve file.",

	// Collection errors
	"collectionsNotFound": "Collections could not be found.",
	"collectionNotFound":  "Collection could not be found.",
	"collectionNotUnique": "Collection name must be unique.",

	// Session errors
	"sessionsNotFound": "Sessions could not be found.",
	"sessionNotFound":  "Session could not be found.",
	"sessionNotUnique": "Session name must be unique.",

	// Comment errors
	"commentsNotFound":       "Comments could not be found.",
	"commentNotFound":        "Comment could not be found.",
	"commentContextNotFound": "The comment's contextId could not be found.",

	// Raw Data Preset
	"rawDataPresetNotUnique": "Raw Data Preset name must be unique.",
	"rawDataPresetNotValid":  "Raw Data Preset is not valid.",
	"rawDataPresetNotFound":  "Raw Data Preset not found.",

	// Chart Preset
	"chartPresetNotUnique": "Chart Preset name must be unique.",
	"chartPresetNotValid":  "Chart Preset was not valid. Ensure posted Sensors exist.",
	"chartPresetNotFound":  "Chart Preset was not found.",

	// Datum
	"datumNotFound": "Datum could not be found.",

	// Authorization
	"malformedToken":   "Token is malformed.",
	"expiredToken":     "Token is expired.",
	"invalidatedToken": "Token has been invalidated.",
	"invalidToken":     "Could not handle token.",
	"emptyToken":       "Token was an empty string.",

	// File error
	"notCSV":                 "Not a CSV file.",
	"noFileRcvd":             "No file received.",
	"couldNotUploadFile":     "Could not upload file.",
	"fileNotFound":           "File not found.",
	"subfolderCreationError": "Could not create subfolder.",
	"failedToDeleteFile":     "Failed to delete file associated with session.",
	"failedToDeleteFiles":    "Failed to delete files associated with thing.",
	"failedToRenameFile":     "Failed to rename the session's file.",
}
