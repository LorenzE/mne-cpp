#pragma once

#include <QtGui/QPixmap>

#include <nodes/NodeDataModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class ScalingData : public NodeData
{
public:

    ScalingData() {}

    ScalingData(const QMap<qint32, float>& scalingMap)
    : m_scalingMap(scalingMap)
    {}

    NodeDataType
    type() const override
    {
        //       id      name
        return {"channelScaling", "cS"};
    }

    QMap<qint32, float> scalingMap() const
    { return m_scalingMap; }

private:

    QMap<qint32, float> m_scalingMap;
};
