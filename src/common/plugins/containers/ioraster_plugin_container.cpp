#include "ioraster_plugin_container.h"

IORasterPluginContainer::IORasterPluginContainer()
{
}

size_t IORasterPluginContainer::size() const
{
	return ioRasterPlugins.size();
}

void IORasterPluginContainer::clear()
{
	ioRasterPlugins.clear();
	inputRasterFormatToPluginMap.clear();
}

void IORasterPluginContainer::pushIORasterPlugin(IORasterPlugin* iIORaster)
{
	ioRasterPlugins.push_back(iIORaster);

	//add input formats to inputFormatMap
	for (const FileFormat& ff : iIORaster->importFormats()){
		for (QString currentExtension : ff.extensions) {
			if (! inputRasterFormatToPluginMap.contains(currentExtension.toLower())) {
				inputRasterFormatToPluginMap.insert(currentExtension.toLower(), iIORaster);
			}
		}
	}
}

void IORasterPluginContainer::eraseIORasterPlugin(IORasterPlugin* iIORaster)
{
	ioRasterPlugins.erase(std::find(ioRasterPlugins.begin(), ioRasterPlugins.end(), iIORaster));
	for (const FileFormat& ff : iIORaster->importFormats()){
		for (QString currentExtension : ff.extensions) {
			inputRasterFormatToPluginMap.remove(currentExtension.toLower());
		}
	}
}

IORasterPlugin* IORasterPluginContainer::inputRasterPlugin(const QString inputFormat) const
{
	auto it = inputRasterFormatToPluginMap.find(inputFormat.toLower());
	if (it != inputRasterFormatToPluginMap.end())
		return *it;
	return nullptr;
}

bool IORasterPluginContainer::isInputRasterFormatSupported(const QString inputFormat) const
{
	return inputRasterFormatToPluginMap.find(inputFormat.toLower()) != inputRasterFormatToPluginMap.end();
}

QStringList IORasterPluginContainer::inputRasterFormatList() const
{
	return inputRasterFormatToPluginMap.keys();
}

IORasterPluginContainer::IORasterPluginRangeIterator IORasterPluginContainer::ioRasterPluginIterator(bool iterateAlsoDisabledPlugins) const
{
	return IORasterPluginRangeIterator(this, iterateAlsoDisabledPlugins);
}

ConstPluginIterator<IORasterPlugin> IORasterPluginContainer::IORasterPluginRangeIterator::begin()
{
	return ConstPluginIterator<IORasterPlugin>(pm->ioRasterPlugins, pm->ioRasterPlugins.begin(), b);
}

ConstPluginIterator<IORasterPlugin> IORasterPluginContainer::IORasterPluginRangeIterator::end()
{
	return ConstPluginIterator<IORasterPlugin>(pm->ioRasterPlugins, pm->ioRasterPlugins.end(), b);
}

IORasterPluginContainer::IORasterPluginRangeIterator::IORasterPluginRangeIterator(
		const IORasterPluginContainer* pm,
		bool iterateAlsoDisabledPlugins) :
	pm(pm), b(iterateAlsoDisabledPlugins)
{
}
