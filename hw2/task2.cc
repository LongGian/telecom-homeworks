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
#include <string.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("HW2_Task2_Team_23");

int main (int argc, char* argv[]) {
    bool useRtsCts = false;
    bool verbose = false;
    bool useNetAnim = false;
    uint32_t nWifi = 5;
    std::string ssid= "TLC2022";
    
    CommandLine cmd(__FILE__);

    cmd.AddValue("useRtsCts", "rts threshold -> can be true or false", useRtsCts);
    cmd.AddValue("verbose", "verbose for log on/off -> can be true or false", verbose);
    cmd.AddValue("useNetAnim", "netAnim on/off -> can be true or false", useNetAnim);
    cmd.AddValue("ssid", "Set Ssid -> somma delle matricole", ssid);
 
    cmd.Parse(argc, argv);

    std::string correctSSID = std::to_string(1890490+1938462+1933472+1965768);
    try {
        if (correctSSID.compare(ssid) != 0)
            throw (ssid);
    } 
    catch (std::string wrongSSID){
        std::cerr << "exception caught: " << wrongSSID << "\n";
        exit(EXIT_FAILURE);
    }

    if (verbose) {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    UintegerValue ctsThreshold = (useRtsCts ? UintegerValue(100) : UintegerValue(2346));
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThreshold);

    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(nWifi);
    NodeContainer wifiApNode;
    wifiApNode.Create(1);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());

    WifiMacHelper mac;
    Ssid nome_Ssid=Ssid(ssid);

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211g);
    wifi.SetRemoteStationManager("ns3::AarfWifiManager");
    
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(nome_Ssid), "ActiveProbing", BooleanValue(false));
    NetDeviceContainer staDevices;
    staDevices = wifi.Install(phy, mac, wifiStaNodes);

    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(nome_Ssid));
    NetDeviceContainer apDevices;
    apDevices = wifi.Install(phy, mac, wifiApNode);

    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX", DoubleValue(0.0), "MinY",DoubleValue(0.0),"DeltaX",DoubleValue(5.0),"DeltaY",DoubleValue(10.0),"GridWidth",UintegerValue(3),"LayoutType",StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue(Rectangle(-90,90,-90,90)));
    mobility.Install(wifiStaNodes);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiApNode);

    InternetStackHelper stack;
    stack.Install(wifiStaNodes);
    stack.Install(wifiApNode);

    Ipv4AddressHelper address;
    
    address.SetBase("192.168.1.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterface;
    wifiInterface = address.Assign(staDevices);
    Ipv4InterfaceContainer apInterface;
    apInterface = address.Assign(apDevices);
    
    uint32_t echoServerPort = 21;
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
    echoClient_n4.SetAttribute("Interval", TimeValue(Seconds(3.0)));
    echoClient_n4.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer clientApps_n4 = echoClient_n4.Install(wifiStaNodes.Get(4));
    clientApps_n4.Start(Seconds(1.0));
    clientApps_n4.Stop(Seconds(5.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    
    phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11);

    std::string filename = "wireless-task2-rts-off.xml";
    std::string pcap_filename = "task2-off-4.pcap";
    std::string ap_filename = "task2-off-5.pcap";

    if (useRtsCts) {
        filename = "wireless-task2-rts-on.xml";
        pcap_filename = "task2-on-4.pcap";
        ap_filename = "task2-on-5.pcap";
    }

    phy.EnablePcap(pcap_filename, staDevices.Get(4), true, true);
    phy.EnablePcap(ap_filename, apDevices.Get(0), true, true);

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

        anim.UpdateNodeDescription(wifiApNode.Get(0), "AP-5");
        anim.UpdateNodeColor(wifiApNode.Get(0), 66, 49, 137);

        anim.EnablePacketMetadata(); 
            
        anim.EnableWifiMacCounters(Seconds(0), Seconds(10)); 
        anim.EnableWifiPhyCounters(Seconds(0), Seconds(10)); 
    }

        Simulator::Stop(Seconds(10.0));
        Simulator::Run();
        Simulator::Destroy();
        
    return 0;
}
