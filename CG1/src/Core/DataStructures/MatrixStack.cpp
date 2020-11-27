#include "MatrixStack.hpp"

MatrixStack::MatrixStack()
	: _translation { glm::mat4(1.f)                             }
	, _rotation    { glm::mat4(1.f)                             }
	, _scale       { glm::scale(glm::mat4(1.f), glm::vec3(1.f)) }
{
	_stack.push(glm::mat4(1.f));
}

void MatrixStack::translate(float x, float y, float z)
{
	auto tmp = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));

	_stack.top() *= tmp;
	_translation *= tmp;
}

void MatrixStack::rotate(float degree, float x, float y, float z)
{
	auto tmp = glm::rotate(glm::mat4(1.f), glm::radians(degree), glm::vec3(x, y, z));

	_stack.top() *= tmp;
	_rotation *= tmp;
}

void MatrixStack::scale(float x, float y, float z)
{
	auto tmp = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));

	_stack.top() *= tmp;
	_scale *= tmp;
}

void MatrixStack::setPosition(float x, float y, float z)
{
	_translation = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
	recalculate();
}

void MatrixStack::setRotation(float anglex, float angley, float anglez, const glm::mat4& pivot)
{
	_rotation =
		  glm::rotate(glm::mat4(1.f), glm::radians(anglex), glm::vec3(pivot[0][0], pivot[0][1], pivot[0][2]))
		* glm::rotate(glm::mat4(1.f), glm::radians(angley), glm::vec3(pivot[1][0], pivot[1][1], pivot[1][2]))
		* glm::rotate(glm::mat4(1.f), glm::radians(anglez), glm::vec3(pivot[2][0], pivot[2][1], pivot[2][2]));
	recalculate();
}

void MatrixStack::setScale(float x, float y, float z)
{
	_scale = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));
	recalculate();
}