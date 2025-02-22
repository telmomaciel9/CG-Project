#include "headers/Parser.h"
#include "headers/rapidxml.hpp"

#include <string>
#include <regex>

using namespace std;
using namespace rapidxml;

int Parser::parseWindow (rapidxml::xml_node<>* windowNode)
{
    xml_attribute<>* width = windowNode->first_attribute("width");
    xml_attribute<>* height = windowNode->first_attribute("height");
    
    if (!width || !height) return 2;

    this->window = pair<float, float>(strtod(width->value(), NULL), strtod(height->value(),NULL));
    
    return 0;
}

int Parser::parseCamera (rapidxml::xml_node<>* cameraNode)
{
    xml_attribute<> *x, *y, *z;

    // Parsing camera position
    xml_node<>* node = cameraNode->first_node("position");

    if (node)
    {
        x = node->first_attribute("x");
        y = node->first_attribute("y");
        z = node->first_attribute("z");

        if (!x || !y || !z)
        {
            cerr << "Invalid Camera Position" << endl;
            return 2;
        }

        this->camera->setPosition(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
    }

    node = cameraNode->first_node("lookAt");

    if (node)
    {
        x = node->first_attribute("x");
        y = node->first_attribute("y");
        z = node->first_attribute("z");

        if (!x || !y || !z)
        {
            cerr << "Invalid Camera LookAt" << endl;
            return 2;
        }
            
        this->camera->setLookAt(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
    }
    
    node = cameraNode->first_node("up");

    if (node)
    {
        x = node->first_attribute("x");
        y = node->first_attribute("y");
        z = node->first_attribute("z");

        if (!x || !y || !z)
        {
            cerr << "Invalid Camera Up" << endl;
            return 2;
        }
            
        this->camera->setUp(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
    }

    node = cameraNode->first_node("projection");

    if (node)
    {
        x = node->first_attribute("fov");
        y = node->first_attribute("near");
        z = node->first_attribute("far");

        if (!x || !y || !z)
        {
            cerr << "Invalid Camera Projection" << endl;
            return 2;
        }

        this->camera->setFov(strtod(x->value(), NULL));
        this->camera->setNear(strtod(y->value(), NULL));
        this->camera->setFar(strtod(z->value(), NULL));
    }

    return 0;
}

int Parser::parseGroup(rapidxml::xml_node<>* groupNode)
{
    xml_node<>* modelsNode = groupNode->first_node("models");

    if (modelsNode)
    {
        xml_node<>* model;
        for (model = modelsNode->first_node("model"); model; model = model->next_sibling())
        {
            xml_attribute<>* file = model->first_attribute("file");

            if (!file)
                return 2;
            
            char* filename = file->value();
            if (this->parseModel(filename))
                return 1;
        }
    }

    return 0;
}

int Parser::parseXML(char *filePath)
{
    xml_document<> document;
    xml_node<> * root_node = NULL;

	string path = "../files/";
	path.append(filePath);
    ifstream file(path);
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    document.parse<0>(&buffer[0]);

    root_node = document.first_node("world");

    if (!root_node)
    {
        cerr << "Erro ao ler ficheiro" << std::endl;
        return 1;
    }

    int rv = this->parseWindow(root_node->first_node("window"));
    rv = rv | this->parseCamera(root_node->first_node("camera"));
    rv = rv | this->parseGroup(root_node->first_node("group"));

    file.close();

    if (rv)
        return rv;

    return 0;
}

int Parser::parseModel(char* filename)
{
	string path = "../files/";
	path.append(filename);
    ifstream modelFile(path);

	if (modelFile.bad())
	{
		cerr << "Error opening file \"" << filename << "\".\nMake sure the file exists...";
		return 1;
	}

    string line;

    Shape *forma = new Shape();

    while(getline(modelFile, line))
    {
        float x,y,z;
        if (sscanf(line.c_str(), "%f, %f, %f", &x, &y, &z) != 3)
		{
			cerr << "Error: model line has wrong syntax...\n Make sure the file was generated correctly..";
            return 1;
		}

        forma->addPoint(Point(x,y,z));
    }

    this->models.push_back(forma);

    return 0;
}

std::pair<float, float> Parser::getWindow()
{
    return this->window;
}

Camera* Parser::getCamera()
{
    return this->camera;
}

std::vector<Shape*> Parser::getModels()
{
    std::vector<Shape*> dup(this->models);
    return dup;
}
