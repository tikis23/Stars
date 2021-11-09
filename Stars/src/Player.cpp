#include "Player.h"
#include <iostream>
#include "ImGuiWindow.h"
Player::Player(Window* window)
{
	this->window = window;
	camera = new Camera;
	Input::AddWindow(window->GetHandle());
}

Player::~Player()
{
	Input::RemoveWindow(window->GetHandle());
}

void Player::UpdateInput()
{
    float deltaTime = glfwGetTime() - previousTime;
    previousTime = glfwGetTime();
    if (*ImGuiWindow::Variable_bool("Freecam"))
    {
        if (Input::GetMouseButton(window->GetHandle(), Input::Mouse::RIGHT, Input::Action::HOLD))
        {
            glm::vec2 currentMouse = Input::GetMousePosition(window->GetHandle());
            glm::vec2 diff = currentMouse - previousMouse;
            diff *= sensitivity * deltaTime;
            camera->OffsetRotation(diff.x, diff.y);
        }
        glm::vec3 offset = camera->GetFront();
        offset.y = 0;
        offset = glm::normalize(offset);
        float mult = 1;
        if (Input::GetKey(window->GetHandle(), Input::Key::LEFT_CONTROL, Input::Action::HOLD))
            mult = 10;
        if (Input::GetKey(window->GetHandle(), Input::Key::W, Input::Action::HOLD))
            camera->OffsetPosition(offset * speed * mult * deltaTime);
        if (Input::GetKey(window->GetHandle(), Input::Key::S, Input::Action::HOLD))
            camera->OffsetPosition(-offset * speed * mult * deltaTime);
        if (Input::GetKey(window->GetHandle(), Input::Key::A, Input::Action::HOLD))
            camera->OffsetPosition(glm::normalize(glm::cross(offset, -camera->GetUp())) * speed * mult * deltaTime);
        if (Input::GetKey(window->GetHandle(), Input::Key::D, Input::Action::HOLD))
            camera->OffsetPosition(glm::normalize(glm::cross(offset, camera->GetUp())) * speed * mult * deltaTime);
        if (Input::GetKey(window->GetHandle(), Input::Key::SPACE, Input::Action::HOLD))
            camera->OffsetPosition(camera->GetUp() * speed * mult * deltaTime);
        if (Input::GetKey(window->GetHandle(), Input::Key::LEFT_SHIFT, Input::Action::HOLD))
            camera->OffsetPosition(-camera->GetUp() * speed * mult * deltaTime);
        camera->Update(window->GetAspectRatio(), false);
    }
    else
    {
        if (orbitSpeed < 0.0025f)
            orbitSpeed = 0.0025f;
        orbitDist -= orbitSpeed * Input::GetScrollOffsetY(0U);
        if (orbitDist < orbitMin)
            orbitDist = orbitMin;  
        if (Input::GetMouseButton(window->GetHandle(), Input::Mouse::RIGHT, Input::Action::HOLD))
        {
            glm::vec2 currentMouse = Input::GetMousePosition(window->GetHandle());
            glm::vec2 diff = currentMouse - previousMouse;
            diff *= sensitivity * deltaTime;
            camera->OffsetRotation(diff.x, -diff.y);
        }
        float yaw, pitch;
        camera->GetRotation(yaw, pitch);
        glm::vec3 pos;
        pos.x = orbitDist * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        pos.y = orbitDist * sin(glm::radians(pitch));
        pos.z = orbitDist * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera->SetPosition(pos + orbitPos);
        camera->Update(window->GetAspectRatio(), true, orbitPos);
    }
    previousMouse = Input::GetMousePosition(window->GetHandle());
    Input::Poll();
}