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
        if (*ImGuiWindow::Variable_int("SmoothMove") == 0)
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
    }
    else
    {
        if (*ImGuiWindow::Variable_int("SmoothMove") == 0)
        {
            if (orbitSpeed < 0.0025f)
                orbitSpeed = 0.0025f;
            if (Input::GetKey(0U, Input::Key::LEFT_CONTROL, Input::Action::HOLD))
                orbitDist -= orbitSpeed * Input::GetScrollOffsetY(0U) * scrollSensitivity * 10;
            else
                orbitDist -= orbitSpeed * Input::GetScrollOffsetY(0U) * scrollSensitivity;
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
    }
    previousMouse = Input::GetMousePosition(window->GetHandle());
    Input::Poll();
}

void Player::SmoothMove()
{
    if (*ImGuiWindow::Variable_int("SmoothMove") > 0)
    {
        int currentFrame = Global::maxFrames - *ImGuiWindow::Variable_int("SmoothMove");
        float sigmoid = (1.0f + atanf(((float)currentFrame / Global::maxFrames - 0.5f) * 3.0f)) * 0.505f;
        if (currentFrame < 1)
        {
            oldPos = camera->GetPosition();
            orbitPosLast = orbitPos;
        }
        oldPos += orbitPos - orbitPosLast;
        // smooth pos
        glm::vec3 forward = (orbitPos - oldPos) + (orbitDist * glm::normalize(oldPos - orbitPos));
        camera->SetPosition(oldPos + forward * sigmoid);

        // orbit pos
        glm::vec3 pos = glm::normalize(orbitPos - oldPos);
        float y = -glm::degrees(atan2(pos.x, pos.z));
        float p = -glm::degrees(asin(pos.y));
        y -= 90;
        if (y < 0)
            y += 360;
        camera->SetRotation(y, p);

        // smooth rotation
        glm::vec3 point = oldOrbitPos + (orbitPos - oldOrbitPos) * (atanf((float)currentFrame / Global::maxFrames * 3.0f) * 0.8f);

        camera->Update(window->GetAspectRatio(), true, point);
        *ImGuiWindow::Variable_int("SmoothMove") = *ImGuiWindow::Variable_int("SmoothMove") - 1;
    }
    else
        oldOrbitPos = orbitPos;
    orbitPosLast = orbitPos;
}
