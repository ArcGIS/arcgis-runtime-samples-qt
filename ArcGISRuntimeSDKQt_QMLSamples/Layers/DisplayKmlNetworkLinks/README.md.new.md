# Display KML network links

KML files can reference other KML files on the network and support
automatically refreshing content. This sample demonstrates how to
display a file with a network link, including displaying any network
link control messages at launch.

![](screenshot.png)

## How to use the sample

The sample will load the KML file automatically. The data shown should
refresh automatically every few seconds.

The message button will show the last message received from the KML
network resource.

## How it works

In general, no special code is needed to support showing KML/KMZ files
with network links. KML supports specifying a message to show the user
when loading a network link. Apps can optionally listen to the
`KmlDataset::kmlNetworkLinkMessageReceived` event to show any messages
from KML network link controls.

## About the data

This map shows the current air traffic in parts of Europe with heading,
altitude, and ground speed. Additionally, noise levels from ground
monitoring stations are shown.
