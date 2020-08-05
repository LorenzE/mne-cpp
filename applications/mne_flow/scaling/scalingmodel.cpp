#include "scalingmodel.h"
#include "../data/scalingdata.h"

#include <QtCore/QEvent>
#include <QtCore/QDir>

#include <QtWidgets/QFileDialog>

using namespace DISPLIB;

ScalingModel::ScalingModel()
: m_pScalingView(new ScalingView("MNE_Flow"))
{
    connect(m_pScalingView, &ScalingView::scalingChanged,
            this, &ScalingModel::onScalingChanged);
}


unsigned int ScalingModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In:
        result = 0;
        break;

    case PortType::Out:
        result = 1;

    default:
        break;
    }

    return result;
}


NodeDataType ScalingModel::dataType(PortType, PortIndex) const
{
    return ScalingData().type();
}


std::shared_ptr<NodeData> ScalingModel::outData(PortIndex)
{
    return std::make_shared<ScalingData>(m_scalingMap);
}


void ScalingModel::onScalingChanged(const QMap<qint32, float>& scalingMap)
{
    m_scalingMap = scalingMap;
    emit dataUpdated(0);
}
