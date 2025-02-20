/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005-2021                                           \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

#ifndef MESHLAB_PLUGIN_MANAGER_H
#define MESHLAB_PLUGIN_MANAGER_H

#include "containers/decorate_plugin_container.h"
#include "containers/edit_plugin_container.h"
#include "containers/filter_plugin_container.h"
#include "containers/iomesh_plugin_container.h"
#include "containers/ioraster_plugin_container.h"
#include "containers/render_plugin_container.h"

#include<QMap>
#include<QObject>
#include <QDir>

/**
 * @brief The PluginManager class provides the basic tools for managing all the plugins.
 */
class PluginManager
{
public:
	PluginManager();
	~PluginManager();

	class PluginRangeIterator;

	/** Member functions **/
	static void checkPlugin(const QString& filename);

	void loadPlugins();
	void loadPlugins(QDir pluginsDirectory);
	void loadPlugin(const QString& filename);
	void unloadPlugin(MeshLabPluginFile* ifp);

	void enablePlugin(MeshLabPluginFile* ifp);
	void disablePlugin(MeshLabPluginFile* ifp);

	unsigned int size() const;
	int numberIOPlugins() const;

	DecoratePlugin* getDecoratePlugin(const QString& name);

	QAction* filterAction(const QString& name);

	IOMeshPlugin* inputMeshPlugin(const QString& inputFormat) const;
	IOMeshPlugin* outputMeshPlugin(const QString& outputFormat) const;
	IORasterPlugin* inputRasterPlugin(const QString inputFormat) const;
	bool isInputMeshFormatSupported(const QString inputFormat) const;
	bool isOutputMeshFormatSupported(const QString outputFormat) const;
	bool isInputRasterFormatSupported(const QString inputFormat) const;
	QStringList inputMeshFormatList() const;
	QStringList outputMeshFormatList() const;
	QStringList inputRasterFormatList() const;
	QStringList inputMeshFormatListDialog() const;
	QStringList outputMeshFormatListDialog() const;
	QStringList inputRasterFormatListDialog() const;
	
	MeshLabPluginFile* operator [](unsigned int i) const;

	/** Member functions for range iterators **/
	PluginRangeIterator pluginIterator(bool iterateAlsoDisabledPlugins = false) const;
	FilterPluginContainer::FilterPluginRangeIterator filterPluginIterator(bool iterateAlsoDisabledPlugins = false) const;
	IOMeshPluginContainer::IOMeshPluginRangeIterator ioMeshPluginIterator(bool iterateAlsoDisabledPlugins = false) const;
	IORasterPluginContainer::IORasterPluginRangeIterator ioRasterPluginIterator(bool iterateAlsoDisabledPlugins = false) const;
	RenderPluginContainer::RenderPluginRangeIterator renderPluginIterator(bool iterateAlsoDisabledPlugins = false) const;
	DecoratePluginContainer::DecoratePluginRangeIterator decoratePluginIterator(bool iterateAlsoDisabledPlugins = false) const;
	EditPluginContainer::EditPluginFactoryRangeIterator editPluginFactoryIterator(bool iterateAlsoDisabledPlugins = false) const;

private:
	//all plugins
	std::vector<MeshLabPluginFile*> allPlugins;
	std::set<QString> pluginFiles; //used to check if a plugin file has been already loaded

	//Plugin containers: used for better organization of each type of plugin
	// note: these containers do not own any plugin. Plugins are owned by the PluginManager
	IOMeshPluginContainer ioMeshPlugins;
	IORasterPluginContainer ioRasterPlugins;
	FilterPluginContainer filterPlugins;
	RenderPluginContainer renderPlugins;
	DecoratePluginContainer decoratePlugins;
	EditPluginContainer editPlugins;

	static void checkFilterPlugin(FilterPlugin* iFilter);

	template <typename RangeIterator>
	static QStringList inputFormatListDialog(RangeIterator iterator);

	template <typename RangeIterator>
	static QStringList outputFormatListDialog(RangeIterator iterator);
};

class PluginManager::PluginRangeIterator
{
	friend class PluginManager;
public:
	ConstPluginIterator<MeshLabPluginFile> begin();
	ConstPluginIterator<MeshLabPluginFile> end();
private:
	PluginRangeIterator(const PluginManager* pm, bool iterateAlsoDisabledPlugins = false);
	const PluginManager* pm;
	bool b;
};

#endif // MESHLAB_PLUGIN_MANAGER_H
