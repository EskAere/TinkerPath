#pragma once
#include <map>
#include <memory> 
#include <string>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void					load(Identifier id, const std::string& filename);
	template<typename Parameter>
	void					load(Identifier id, const std::string& filename, const Parameter& secondParam); //Overload for shaders and partial Texture
	Resource&				get(Identifier id);
	const Resource&			get(Identifier id) const;

private:
	void					insertResource(Identifier id, std::unique_ptr<Resource> resource);
private:
	std::map < Identifier, std::unique_ptr<Resource>> mResourceMap;

};



template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string & filename)
{
	std::unique_ptr<Resource> resource(new Resource());	//Create a new Resource
	if (!resource->loadFromFile(filename)) {		//Throw exception if loading issue
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}

	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string & filename, const Parameter& secondParam)
{
	std::unique_ptr<Resource> resource(new Resource());	//Create a new Resource
	if (!resource->loadFromFile(filename, secondParam)) {		//Throw exception if loading issue
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}

	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource & ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end()); //Avoid bug : The requested resource doesn't exist in the map

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}


template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	// Insert and check success
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource))); //Move the resource in our map
	assert(inserted.second); //Avoid bug : Insertion of an existing ID
}
