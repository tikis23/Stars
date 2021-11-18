#include "Planet.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <thread>
Planet::Planet(float i_size, int i_detail, float i_mult, glm::vec3 i_color, glm::vec3 i_planetRot, glm::vec3 i_atmoRot, int i_type, glm::vec3 i_ringColor, bool i_hasRing)
{
    variables.hasRing = i_hasRing;
    if(i_detail > 0)
        variables.detail = i_detail;
    variables.mult = i_mult;
    if (i_mult < 0)
        variables.mult = 1000;
    variables.size = i_size;
    variables.planetRot = i_planetRot;
    variables.ringRot   = i_atmoRot;

    if (i_type == 0)
    {
        variables.isSun = true;
        variables.detail = 5;
    }
    if (i_type == 1)
        variables.isPlanet = true;
    if (i_type == 2)
        variables.isGas = true;
    if (i_type == 3)
        variables.isMoon = true;

    if (variables.isMoon)
    {
        variables.dist = 0.3f;
        variables.speed = 30;
        variables.origin = 0;
    }
    variables.color = glm::vec4(i_color, 1.0f);
    variables.colorRing = glm::vec4(i_ringColor, 0.95f);
    drawData.emplace_back(new _drawData_);
    drawData.emplace_back(new _drawData_);

    std::thread(&Planet::Generate, this).detach();

    GenOrbitData();
}

Planet::Planet(PlanetVariables variables)
{
    this->variables = variables;
    drawData.emplace_back(new _drawData_);
    drawData.emplace_back(new _drawData_);

    std::thread(&Planet::Generate, this).detach();
    GenOrbitData();
}

Planet::Planet()
{
    drawData.emplace_back(new _drawData_);
    drawData.emplace_back(new _drawData_);

    std::thread(&Planet::Generate, this).detach();
    GenOrbitData();
}

Planet::~Planet()
{
    delete drawData[0];
    delete drawData[1];
}

void Planet::Regenerate()
{
    delete drawData[0];
    delete drawData[1];
    drawData.clear();
    drawData.emplace_back(new _drawData_);
    drawData.emplace_back(new _drawData_);

    std::thread(&Planet::Generate, this).detach();
}

void Planet::Generate()
{
    for (int i = 1; i <= 5; i++)
    {
        glm::vec3 p1, p2, p3, p4;

        float yaw = i * 72, pitch = 30;
        p1.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        p1.y = sin(glm::radians(pitch));
        p1.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        p1 = glm::normalize(p1);
        yaw = i * 72 - 72;
        p2.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        p2.y = sin(glm::radians(pitch));
        p2.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        p2 = glm::normalize(p2);

        yaw = i * 72 + 36, pitch = -30;
        p3.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        p3.y = sin(glm::radians(pitch));
        p3.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        p3 = glm::normalize(p3);
        yaw = i * 72 - 36;
        p4.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        p4.y = sin(glm::radians(pitch));
        p4.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        p4 = glm::normalize(p4);

        subdivide(glm::vec3{ 0, 0, 0 } + glm::vec3{ 0, 1, 0 }, glm::vec3{ 0, 0, 0 } + p1, glm::vec3{ 0, 0, 0 } + p2, variables.detail, drawData[0]->data);
        subdivide(glm::vec3{ 0, 0, 0 } + p2, glm::vec3{ 0, 0, 0 } + p1, glm::vec3{ 0, 0, 0 } + p4, variables.detail, drawData[0]->data);
        subdivide(glm::vec3{ 0, 0, 0 } + glm::vec3{ 0, -1, 0 }, glm::vec3{ 0, 0, 0 } + p4, glm::vec3{ 0, 0, 0 } + p3, variables.detail, drawData[0]->data);
        subdivide(glm::vec3{ 0, 0, 0 } + p3, glm::vec3{ 0, 0, 0 } + p4, glm::vec3{ 0, 0, 0 } + p1, variables.detail, drawData[0]->data);
    }
    // ring
    if (variables.hasRing)
    {
        float ringDetail = 5000;
        float ringDensity = 0.001f;


        for (float r = 1; r < 1.5; r += ringDensity)
        {
            glm::vec3 rand1 = glm::vec3{ 1, 1, 1 } *((noise.GetNoise(sinf(glm::radians(0.0f)) / r * ringDetail, cosf(glm::radians(0.0f)) / r * ringDetail) + 1) / 2);
            glm::vec3 rand2 = glm::vec3{ 1, 1, 1 } *((noise.GetNoise(sinf(glm::radians(1.0f)) / (r + ringDensity) * ringDetail, cosf(glm::radians(1.0f)) / (r + ringDensity) * ringDetail) + 1) / 2);
            for (float j = 0; j < 360; j += 3)
            {
                glm::vec3 pos1 = { sinf(glm::radians(j)) / r, 0, cosf(glm::radians(j)) / r };
                glm::vec3 pos2 = { sinf(glm::radians(j + 3)) / r, 0, cosf(glm::radians(j + 3)) / r };
                glm::vec3 pos3 = { sinf(glm::radians(j)) / (r + ringDensity), 0, cosf(glm::radians(j)) / (r + ringDensity) };
                glm::vec3 pos4 = { sinf(glm::radians(j + 3)) / (r + ringDensity), 0, cosf(glm::radians(j + 3)) / (r + ringDensity) };

                // top
                drawData[1]->data.push_back({ pos1, rand1 });
                drawData[1]->data.push_back({ pos2, rand1 });
                drawData[1]->data.push_back({ pos3, rand2 });
                         
                drawData[1]->data.push_back({ pos2, rand1 });
                drawData[1]->data.push_back({ pos4, rand2 });
                drawData[1]->data.push_back({ pos3, rand2 });
                         
                // bottom                   
                drawData[1]->data.push_back({ pos2, rand1 });
                drawData[1]->data.push_back({ pos1, rand1 });
                drawData[1]->data.push_back({ pos3, rand2 });
                         
                drawData[1]->data.push_back({ pos4, rand2 });
                drawData[1]->data.push_back({ pos2, rand1 });
                drawData[1]->data.push_back({ pos3, rand2 });
            }
        }
    }
    generated = true;
}

void Planet::GenOrbitData()
{
    if (orbitData == nullptr)
        orbitData = new _drawData_;
    if (orbitData->drawSize == 0)
    {
        orbitData->bind();
        for (int i = 0; i < 360; i += 1)
        {
            orbitData->data.push_back({ {cos(glm::radians((float)i)), 0, sin(glm::radians((float)i))}, {1, 1, 1} });
            orbitData->data.push_back({ {cos(glm::radians((float)i + 1)), 0, sin(glm::radians((float)i + 1))}, {1, 1, 1} });
        }
        glBufferData(GL_ARRAY_BUFFER, orbitData->data.size() * sizeof(Vertex), &orbitData->data[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        orbitData->drawSize = orbitData->data.size();
        orbitData->data.clear();
    }
}

void Planet::Draw(Shader* shader)
{
    shader->Use();
    if (generated)
    {
        drawData[0]->bind();
        glBufferData(GL_ARRAY_BUFFER, drawData[0]->data.size() * sizeof(Vertex), &drawData[0]->data[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        drawData[0]->drawSize = drawData[0]->data.size();
        drawData[0]->data.clear();
        if (variables.hasRing)
        {
            drawData[1]->bind();
            glBufferData(GL_ARRAY_BUFFER, drawData[1]->data.size() * sizeof(Vertex), &drawData[1]->data[0], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            drawData[1]->drawSize = drawData[1]->data.size();
            drawData[1]->data.clear();
        }
        generated = false;
    }
    model = glm::translate(glm::mat4(1), pos);
    model = glm::scale(model, glm::vec3(variables.size, variables.size, variables.size));
    model = glm::rotate(model, glm::radians(Global::time * 3), variables.planetRot);

    modelAtmo = glm::translate(glm::mat4(1), pos);
    modelAtmo = glm::scale(modelAtmo, glm::vec3(variables.ringSize, variables.ringSize, variables.ringSize));
    modelAtmo = glm::rotate(modelAtmo, glm::radians(Global::time), variables.ringRot);

    shader->setUniform1f("u_time", glfwGetTime());
    shader->setUniform1f("u_minHeight", variables.min_height);
    shader->setUniform1f("u_maxHeight", variables.max_height);
    shader->setUniform1i("u_hasRing", variables.hasRing);
    shader->setUniform1i("u_isSun", variables.isSun);
    shader->setUniform1i("isRing", false);
    shader->setUniform1i("u_isPlanet", variables.isPlanet);
    shader->setUniform4f("u_color", variables.color.x, variables.color.y, variables.color.z, variables.color.w);
    shader->setUniformMatrix4fv("model", glm::value_ptr(model));
    drawData[0]->draw();
    if (variables.hasRing)
    {
        shader->setUniform1i("isRing", true);
        shader->setUniform1i("u_isPlanet", false);
        shader->setUniform4f("u_color", variables.colorRing.x, variables.colorRing.y, variables.colorRing.z, variables.colorRing.w);
        shader->setUniformMatrix4fv("model", glm::value_ptr(modelAtmo));
        drawData[1]->draw();
    }
}

void Planet::DrawOrbit(Shader* shader)
{
    orbitData->bind();
    if (orbitData->drawSize == 0)
        return;
    shader->setUniformMatrix4fv("model", glm::value_ptr(modelOrbit));
    glBindVertexArray(orbitData->VAO);
    glDrawArrays(GL_LINES, 0, orbitData->drawSize);
}

void Planet::SetPos(glm::vec3 pos)
{
    this->pos = pos;
}

glm::vec3 Planet::GetPos()
{
    return pos;
}

void Planet::subdivide(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, int detail, std::vector<Vertex>& data)
{
    if (detail <= 0)
    {
        pos1 = glm::normalize(pos1);
        pos2 = glm::normalize(pos2);
        pos3 = glm::normalize(pos3);
        if (variables.isSun)
        {
            data.push_back(Vertex{ pos1,glm::vec3{1, 1, 1} *((noise.GetNoise(pos1.x * variables.mult, pos1.y * variables.mult, pos1.z * variables.mult) + 1) / 2 + 0.3f) });
            data.push_back(Vertex{ pos2,glm::vec3{1, 1, 1} *((noise.GetNoise(pos2.x * variables.mult, pos2.y * variables.mult, pos2.z * variables.mult) + 1) / 2 + 0.3f) });
            data.push_back(Vertex{ pos3,glm::vec3{1, 1, 1} *((noise.GetNoise(pos3.x * variables.mult, pos3.y * variables.mult, pos3.z * variables.mult) + 1) / 2 + 0.3f) });
        }                               
        else if (variables.isGas)
        {
            data.push_back(Vertex{ pos1, glm::vec3{1, 1, 1} * ((noise.GetNoise(pos1.x , pos1.y * variables.mult, pos1.z) + 1) / 2 + 0.3f) });
            data.push_back(Vertex{ pos2, glm::vec3{1, 1, 1} * ((noise.GetNoise(pos2.x , pos2.y * variables.mult, pos2.z) + 1) / 2 + 0.3f) });
            data.push_back(Vertex{ pos3, glm::vec3{1, 1, 1} * ((noise.GetNoise(pos3.x , pos3.y * variables.mult, pos3.z) + 1) / 2 + 0.3f) });
        }
        else if (variables.isMoon)
        {
            noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
            noise.SetFrequency(0.001f);
            noise.SetFractalType(FastNoiseLite::FractalType_PingPong);

            data.push_back(Vertex{ pos1, glm::vec3{1, 1, 1} * ((noise.GetNoise(pos1.x * variables.mult, pos1.y * variables.mult, pos1.z * variables.mult) + 1) / 2 + 0.3f) });
            data.push_back(Vertex{ pos2, glm::vec3{1, 1, 1} * ((noise.GetNoise(pos2.x * variables.mult, pos2.y * variables.mult, pos2.z * variables.mult) + 1) / 2 + 0.3f) });
            data.push_back(Vertex{ pos3, glm::vec3{1, 1, 1} * ((noise.GetNoise(pos3.x * variables.mult, pos3.y * variables.mult, pos3.z * variables.mult) + 1) / 2 + 0.3f) });
        }
        else
        {
            noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
            noise.SetFractalWeightedStrength(1.5f);
            noise.SetFrequency(0.005f);

            data.push_back(Vertex{ pos1, glm::vec3{1, 1, 1} *((noise.GetNoise(pos1.x * variables.mult, pos1.y * variables.mult, pos1.z * variables.mult) + 1) / 2 ) });
            data.push_back(Vertex{ pos2, glm::vec3{1, 1, 1} *((noise.GetNoise(pos2.x * variables.mult, pos2.y * variables.mult, pos2.z * variables.mult) + 1) / 2 ) });
            data.push_back(Vertex{ pos3, glm::vec3{1, 1, 1} *((noise.GetNoise(pos3.x * variables.mult, pos3.y * variables.mult, pos3.z * variables.mult) + 1) / 2 ) });
        }
    }
    else
    {
        subdivide(pos1, (pos2 + pos1) * 0.5f, (pos3 + pos1) * 0.5f, detail - 1, data);
        subdivide((pos2 + pos1) * 0.5f, pos2, (pos3 + pos2) * 0.5f, detail - 1, data);
        subdivide((pos3 + pos1) * 0.5f, (pos3 + pos2) * 0.5f, pos3, detail - 1, data);
        subdivide((pos2 + pos1) * 0.5f, (pos3 + pos2) * 0.5f, (pos3 + pos1) * 0.5f, detail - 1, data);
    }
}

Planet::_drawData_* Planet::orbitData;
