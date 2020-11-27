#pragma once

#include <stack>
#include <glm/gtc/matrix_transform.hpp>

class MatrixStack
{
private:
	std::stack<glm::mat4> _stack;

	glm::mat4 _translation;
	glm::mat4 _rotation;
	glm::mat4 _scale;
public:

	MatrixStack();
	~MatrixStack() = default;

	void translate(float x, float y, float z);
	void rotate(float degree, float x, float y, float z);
	void scale(float x, float y, float z);

	void setPosition(float x, float y, float z);
	void setRotation(float anglex, float angley, float anglez, const glm::mat4& pivot);
	void setScale(float x, float y, float z);

	inline void recalculate() { _stack.top() = _translation * _scale * _rotation; }
	inline void push() { _stack.push(get()); }
	inline void pop() { if (_stack.size() != 1) _stack.pop(); }
	inline void reset() { while (_stack.size() != 1) pop(); }
	inline const glm::mat4& get() const { return _stack.top(); };
};