#include "ns3/applications-module.h"
#include "ns3/basic-energy-source.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/simple-device-energy-model.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("HW2_Task1_Team_23");

int main (int argc, char* argv[]) {
    bool useRtsCts = false;
    bool verbose = false;
    bool useNetAnim = false;
    uint32_t nWifi = 5;
    
    CommandLine cmd(__FILE__);

    cmd.AddValue("useRtsCts", "rts threshold -> can be true or false", useRtsCts);
    cmd.AddValue("verbose", "verbose for log on/off -> can be true or false", verbose);
    cmd.AddValue("useNetAnim", "netAnim on/off -> can be true or false", useNetAnim);

    cmd.Parse(argc, argv);

    if (verbose) {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    UintegerValue ctsThreshold = (useRtsCts ? UintegerValue(100) : UintegerValue(2346));
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThreshold);

    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(nWifi);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());
    WifiMacHelper mac;

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211g);
    wifi.SetRemoteStationManager("ns3::AarfWifiManager");
    NetDeviceContainer staDevices;
    mac.SetType("ns3::AdhocWifiMac");
    staDevices = wifi.Install(phy, mac, wifiStaNodes);

    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX", DoubleValue(0.0), "MinY",DoubleValue(0.0),"DeltaX",DoubleValue(5.0),"DeltaY",DoubleValue(10.0),"GridWidth",UintegerValue(3),"LayoutType",StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue(Rectangle(-90,90,-90,90)));
    mobility.Install(wifiStaNodes);

    InternetStackHelper stack;
    stack.Install(wifiStaNodes);

    Ipv4AddressHelper address;
    
    address.SetBase("192.168.1.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterface;
    wifiInterface = address.Assign(staDevices);
    
    uint32_t echoServerPort = 20;
    UdpEchoServerHelper echoServer(echoServerPort);

    ApplicationContainer serverApps = echoServer.Install(wifiStaNodes.Get(0));
    serverApps.Start(Seconds(0.0));
    serverApps.Stop(Seconds(5.0));

    UdpEchoClientHelper echoClient_n3(wifiInterface.GetAddress(0), echoServerPort);
    echoClient_n3.SetAttribute("MaxPackets", UintegerValue(2));
    echoClient_n3.SetAttribute("Interval", TimeValue(Seconds(2.0)));
    echoClient_n3.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer clientApps_n3 = echoClient_n3.Install(wifiStaNodes.Get(3));
    clientApps_n3.Start(Seconds(2.0));
    clientApps_n3.Stop(Seconds(5.0));

    UdpEchoClientHelper echoClient_n4(wifiInterface.GetAddress(0), echoServerPort);
    echoClient_n4.SetAttribute("MaxPackets", UintegerValue(2));
    echoClient_n4.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient_n4.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer clientApps_n4 = echoClient_n4.Install(wifiStaNodes.Get(4));
    clientApps_n4.Start(Seconds(1.0));
    clientApps_n4.Stop(Seconds(3.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    
    phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11);

    std::string filename = "wireless-task1-rts-off.xml";
    std::string pcap_filename = "task1-off-2.pcap";

    if (useRtsCts) {
        filename = "wireless-task1-rts-on.xml";
        pcap_filename = "task1-on-2.pcap";
    }

    phy.EnablePcap(pcap_filename, staDevices.Get(2), true, true);

    AnimationInterface anim(filename);

    if (useNetAnim) {
        anim.UpdateNodeDescription(wifiStaNodes.Get(0), "SRV-0");
        anim.UpdateNodeColor(wifiStaNodes.Get(0), 255, 0, 0);

        anim.UpdateNodeDescription(wifiStaNodes.Get(1), "HOC-1");
        anim.UpdateNodeColor(wifiStaNodes.Get(1), 0, 0, 255);

        anim.UpdateNodeDescription(wifiStaNodes.Get(2), "HOC-2");
        anim.UpdateNodeColor(wifiStaNodes.Get(2), 0, 0, 255);

        anim.UpdateNodeDescription(wifiStaNodes.Get(3), "CLI-3");
        anim.UpdateNodeColor(wifiStaNodes.Get(3), 0, 255, 0);

        anim.UpdateNodeDescription(wifiStaNodes.Get(4), "CLI-4");
        anim.UpdateNodeColor(wifiStaNodes.Get(4), 0, 255, 0);

        anim.EnablePacketMetadata(); 
            
        anim.EnableWifiMacCounters(Seconds(0), Seconds(10)); 
        anim.EnableWifiPhyCounters(Seconds(0), Seconds(10)); 
    }

        Simulator::Stop(Seconds(10.0));
        Simulator::Run();
        Simulator::Destroy();

    return 0;
}
