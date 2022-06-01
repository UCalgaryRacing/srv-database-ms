package services

import (
	"database-ms/app/model"
	"database-ms/config"
	"database-ms/utils"

	"github.com/google/uuid"
	"github.com/jackc/pgconn"
	"golang.org/x/net/context"
	"gorm.io/gorm"
)

type CollectionServiceInterface interface {
	// Public
	FindCollectionsByThingId(context.Context, uuid.UUID) ([]*model.Collection, *pgconn.PgError)
	CreateCollection(context.Context, *model.Collection) *pgconn.PgError
	UpdateCollection(context.Context, *model.Collection) *pgconn.PgError
	DeleteCollection(context.Context, uuid.UUID) *pgconn.PgError

	// Comments
	FindCommentsByCollectionId(context.Context, uuid.UUID) ([]*model.CollectionComment, *pgconn.PgError)
	CreateComment(context.Context, *model.CollectionComment) *pgconn.PgError
	UpdateCommentContent(context.Context, *model.CollectionComment) *pgconn.PgError
	DeleteComment(context.Context, uuid.UUID) *pgconn.PgError

	// Private
	FindById(context.Context, uuid.UUID) (*model.Collection, *pgconn.PgError)
	FindCommentById(context.Context, uuid.UUID) (*model.CollectionComment, *pgconn.PgError)
}

type CollectionService struct {
	db     *gorm.DB
	config *config.Configuration
}

func NewCollectionService(db *gorm.DB, c *config.Configuration) CollectionServiceInterface {
	return &CollectionService{config: c, db: db}
}

// PUBLIC COLLECTION FUNCTIONS

func (service *CollectionService) FindCollectionsByThingId(ctx context.Context, thingId uuid.UUID) ([]*model.Collection, *pgconn.PgError) {
	var collections []*model.Collection
	result := service.db.Where("thing_id = ?", thingId).Find(&collections)
	if result.Error != nil {
		return nil, utils.GetPostgresError(result.Error)
	}
	return collections, nil
}

func (service *CollectionService) CreateCollection(ctx context.Context, collection *model.Collection) *pgconn.PgError {
	result := service.db.Create(&collection)
	return utils.GetPostgresError(result.Error)
}

func (service *CollectionService) UpdateCollection(ctx context.Context, updatedCollection *model.Collection) *pgconn.PgError {
	result := service.db.Updates(&updatedCollection)
	return utils.GetPostgresError(result.Error)
}

func (service *CollectionService) DeleteCollection(ctx context.Context, collectionId uuid.UUID) *pgconn.PgError {
	collection := model.Collection{Base: model.Base{Id: collectionId}}
	result := service.db.Delete(&collection)
	return utils.GetPostgresError(result.Error)
}

// PUBLIC COMMENT FUNCTIONS

func (service *CollectionService) FindCommentsByCollectionId(ctx context.Context, collectionId uuid.UUID) ([]*model.CollectionComment, *pgconn.PgError) {
	var comments []*model.CollectionComment
	result := service.db.Where("collection_id = ?", collectionId).Find(&comments)
	if result.Error != nil {
		return nil, utils.GetPostgresError(result.Error)
	}
	return comments, nil
}

func (service *CollectionService) CreateComment(ctx context.Context, comment *model.CollectionComment) *pgconn.PgError {
	result := service.db.Create(&comment)
	return utils.GetPostgresError(result.Error)
}

func (service *CollectionService) UpdateCommentContent(ctx context.Context, updatedComment *model.CollectionComment) *pgconn.PgError {
	result := service.db.Updates(&updatedComment)
	return utils.GetPostgresError(result.Error)
}

func (service *CollectionService) DeleteComment(ctx context.Context, commentId uuid.UUID) *pgconn.PgError {
	comment := model.CollectionComment{Base: model.Base{Id: commentId}}
	result := service.db.Delete(&comment)
	return utils.GetPostgresError(result.Error)
}

// PRIVATE FUNCTIONS

func (service *CollectionService) FindById(ctx context.Context, collectionId uuid.UUID) (*model.Collection, *pgconn.PgError) {
	var collection *model.Collection
	result := service.db.Where("id = ?", collectionId).First(&collection)
	if result.Error != nil {
		return nil, &pgconn.PgError{}
	}
	return collection, nil
}

func (service *CollectionService) FindCommentById(ctx context.Context, commentId uuid.UUID) (*model.CollectionComment, *pgconn.PgError) {
	var comment *model.CollectionComment
	result := service.db.Where("id = ?", commentId).First(&comment)
	if result.Error != nil {
		return nil, utils.GetPostgresError(result.Error)
	}
	return comment, nil
}
