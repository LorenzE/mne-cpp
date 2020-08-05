#pragma once

#include <QtGui/QPixmap>

#include <NodeDataModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class FilePathData : public NodeData
{
public:

    FilePathData() {}

    FilePathData(const QString &sFilePath)
    : m_sFilePath(sFilePath)
    {}

    NodeDataType
    type() const override
    {
        //       id      name
        return {"filePath", "fP"};
    }

    QString filePath() const
    { return m_sFilePath; }

private:

    QString m_sFilePath;
};
