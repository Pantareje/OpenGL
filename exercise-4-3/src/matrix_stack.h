#pragma once

#include <cglm/cglm.h>

#include <assert.h>

#define MAT_STACK_SIZE 20

struct MatrixStack {
	unsigned size;
	mat4 data[MAT_STACK_SIZE];
};

static inline void ms_push(struct MatrixStack *stack, mat4 matrix)
{
	assert(stack->size < MAT_STACK_SIZE);
	glm_mat4_copy(matrix, stack->data[stack->size]);
	stack->size += 1;
}

static inline void ms_add(struct MatrixStack *stack, mat4 matrix)
{
	assert(stack->size > 0);
	assert(stack->size < MAT_STACK_SIZE);
	glm_mat4_mul(stack->data[stack->size - 1], matrix, stack->data[stack->size]);
	stack->size += 1;
}

static inline void ms_pop(struct MatrixStack *stack)
{
	assert(stack->size > 0);
	stack->size -= 1;
}

static inline void ms_clear(struct MatrixStack *stack)
{
	stack->size = 0;
}

static inline float(*ms_top(struct MatrixStack *stack))[4]
{
	assert(stack->size > 0);
	return stack->data[stack->size - 1];
}
