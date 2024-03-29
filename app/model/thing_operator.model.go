package model

import (
	"github.com/google/uuid"
)

const TableNameThingOperator = "thing_operator"

type ThingOperator struct {
	Base
	OperatorId uuid.UUID `gorm:"type:uuid;column:operator_id;not null"`
	ThingId    uuid.UUID `gorm:"type:uuid;column:thing_id;not null"`
	Operator   Operator  `gorm:"constraint:OnUpdate:CASCADE,OnDelete:CASCADE"`
	Thing      Thing     `gorm:"constraint:OnUpdate:CASCADE,OnDelete:CASCADE"`
}

func (*ThingOperator) TableName() string {
	return TableNameThingOperator
}
