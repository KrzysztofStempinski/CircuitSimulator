/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Circuit_fileIO.cpp
// 
//  ---------------------------------------------

// NOTE: this is ugly, and unsafe, lacks error checking
// (and generally is in poor taste)
// HOWEVER, I need file IO methods up and running

// TODO: http://rapidjson.org/md_doc_stream.html, change from i/ofstream to dedicated streams
// TODO: error handling
// TODO(?): check for version compatibility in loadFromFile(...)

#include "Circuit.h"
#include "Version.h"

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/istreamwrapper.h"
#include "../rapidjson/prettywriter.h"

bool Circuit::saveToFile(const QString fileName) {

	// assign each node and component a unique ID, which is essentially its index
	int ID = 0;
	for (auto &it : nodes) {
		it->ID = ID;
		ID++;
	}

	ID = 0;
	for (auto &it : components) {
		it->ID = ID;
		ID++;
	}


	// initialize an emppty JSON doc
	rapidjson::Document file;
	file.SetObject();
	rapidjson::Document::AllocatorType& allocator = file.GetAllocator();

	// save version info for comparision between releases
	rapidjson::Value version(VersionInfo::getVersionString().toStdString().c_str(),
								VersionInfo::getVersionString().toStdString().size(),
								allocator);
	file.AddMember("version", version, allocator);

	// start by saving all nodes to file
	rapidjson::Value arrayNodes(rapidjson::kArrayType);
	for (auto &it : nodes)
		it->saveToJSON(arrayNodes, allocator);	//	if (!it->isCoupled())

	file.AddMember("nodes", arrayNodes, allocator);

	// save all components to file
	rapidjson::Value arrayComponents(rapidjson::kArrayType);
	for (auto &it : components)
		it->saveToJSON(arrayComponents, allocator);

	file.AddMember("components", arrayComponents, allocator);

	// save JSON doc to disk
	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf); // prettywrite so that the file is at least somewhat human-readable
	file.Accept(writer);

	std::ofstream out(fileName.toStdString());
	out << strbuf.GetString();
	out.close();

	return true; 

}

bool Circuit::loadFromFile(const QString fileName) {

	// clean up
	nodes.clear();
	components.clear();

	// load and parse file 
	std::ifstream ifs(fileName.toStdString());

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document file;
	file.ParseStream(isw);

	// read component data and create all components
	const rapidjson::Value& componentsVal = file["components"];
	for (rapidjson::Value::ConstValueIterator it = componentsVal.Begin(); it != componentsVal.End(); ++it) {

		createComponent((*it)["name"].GetString(), QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()), false); // disable automatic node creation

		components.back()->setRotationAngle((*it)["rotationAngle"].GetInt());

		// TODO load properties from file
		if ((*it).HasMember("properties")) {

			const rapidjson::Value& arrayProperties = (*it)["properties"];

			//for (rapidjson::Value::ConstValueIterator itp = arrayProperties.Begin(); itp != arrayProperties.End(); ++itp)
				//components.back()->properties.set
			//for (int i = 0; i < components[components.size() - 1]->properties.getPropertyCount(); i++) {

				//double val = component["properties"][i].GetDouble();

			//	components[components.size() - 1]->properties.setPropertyValue(i, component["properties"][i].GetDouble());

			//}
		}

	}

	const rapidjson::Value& nodesVal = file["nodes"];

	// first, we need to create all the nodes - only then can we start connecting them together
	for (rapidjson::Value::ConstValueIterator it = nodesVal.Begin(); it != nodesVal.End(); ++it) {

		if ((*it).HasMember("coupled")) {
			createNode(QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()), components[(*it)["coupledComponent"].GetInt()]);
			components[(*it)["coupledComponent"].GetInt()]->coupledNodes.push_back(nodes.back());
		} else {
			createNode(QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()));
		}

		//nodes.back()->ID = (*it)["ID"].GetInt();

	}

	// now we're sure that every component has as many coupled nodes as required
	for (const auto& it : components)
		it->updateNodeOffsets();

	// connect nodes together
	for (rapidjson::Value::ConstValueIterator it = nodesVal.Begin(); it != nodesVal.End(); ++it) 
		if ((*it).HasMember("connectedNodes"))  // TODO it should always have one
			for (rapidjson::Value::ConstValueIterator jt = (*it)["connectedNodes"].Begin(); jt != (*it)["connectedNodes"].End(); ++jt) 
				nodes[(*it)["ID"].GetInt()]->connectTo(nodes[(*jt).GetInt()]);

	
	return true;

}
