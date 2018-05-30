#include "onewaymessage.h"
#include "apigateway.h"

OneWayMessage::OneWayMessage(ApiGateway *gateway,
                             MessageType type,
                             const QString &endpoint)
    : ApiMessage(gateway, type),
      m_endpointUrl(endpoint)
{

}

void OneWayMessage::sendMessage()
{
    QNetworkRequest request(buildEndpointAjaxUrl(m_endpointUrl, this->constructedMessageData()));
    buildHeaders(request);

    auto reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &OneWayMessage::finished);
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}
