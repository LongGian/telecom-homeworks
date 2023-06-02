#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include <string>
#include <vector>
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Task_1_Team_23");

int main(int argc, char* argv[]) {
    bool verbose = true;
    CommandLine cmd(__FILE__);
    int configuration;

    cmd.AddValue("configuration", "Number of configuration", configuration);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);

    cmd.Parse(argc, argv);
    

    if (verbose) {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
        LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);
        LogComponentEnable("PacketSink", LOG_LEVEL_INFO);
    }

    // INIZIO TOPOLOGIA
    // creazione nodi prima lan
    uint32_t nCsma_1 = 3;
    NodeContainer n_lan_1;
    n_lan_1.Create(nCsma_1);

    // creazione nodi p2p da n2 a n3
    NodeContainer n3;
    n3.Add(n_lan_1.Get(2));
    n3.Create(1);

    // creazione nodi p2p da n3 a n4
    NodeContainer l0;
    l0.Add(n3.Get(1));
    l0.Create(1);

    // creazione nodi p2p da n3 a n5
    NodeContainer l1;
    l1.Add(n3.Get(1));
    l1.Create(1);

    // creazione nodi seconda lan
    uint32_t nCsma_2 = 3;
    NodeContainer n_lan_2;
    n_lan_2.Create(nCsma_2);

    // creazione nodi p2p da n5 a n6
    NodeContainer l2;
    l2.Add(l1.Get(1));
    l2.Add(n_lan_2.Get(0));

    // creazione nodi p2p da n4 a n6
    NodeContainer l3;
    l3.Add(l0.Get(1));
    l3.Add(n_lan_2.Get(0));

    // creazione prima lan a livello di rete
    CsmaHelper csma_1;
    csma_1.SetChannelAttribute("DataRate", StringValue("25Mbps"));
    csma_1.SetChannelAttribute("Delay", TimeValue(MicroSeconds(10)));
    NetDeviceContainer lan_1 = csma_1.Install(n_lan_1);

    InternetStackHelper stack_lan_1;
    stack_lan_1.Install(n_lan_1);

    Ipv4AddressHelper address_lan_1;
    address_lan_1.SetBase("192.148.1.0", "255.255.255.0");

    Ipv4InterfaceContainer interface_lan_1;
    interface_lan_1 = address_lan_1.Assign(lan_1);

    // creazione p2p da n2 a n3
    PointToPointHelper p2p_n2_n3;
    p2p_n2_n3.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
    p2p_n2_n3.SetChannelAttribute("Delay", TimeValue(MicroSeconds(15)));

    NetDeviceContainer dev_n2_n3;
    dev_n2_n3 = p2p_n2_n3.Install(n3);

    InternetStackHelper stack_n2_n3;
    stack_n2_n3.Install(n3.Get(1));

    Ipv4AddressHelper address_n2_n3;
    address_n2_n3.SetBase("10.1.1.0", "255.255.255.252");

    Ipv4InterfaceContainer interface_n2_n3;
    interface_n2_n3 = address_n2_n3.Assign(dev_n2_n3);

    // creazione p2p da n3 a n4
    PointToPointHelper p2p_n3_n4;
    p2p_n3_n4.SetDeviceAttribute("DataRate", StringValue("80Mbps"));
    p2p_n3_n4.SetChannelAttribute("Delay", TimeValue(MicroSeconds(5)));

    NetDeviceContainer dev_n3_n4;
    dev_n3_n4 = p2p_n3_n4.Install(l0);

    InternetStackHelper stack_n3_n4;
    stack_n3_n4.Install(l0.Get(1));

    Ipv4AddressHelper address_n3_n4;
    address_n3_n4.SetBase("10.0.1.0", "255.255.255.252");

    Ipv4InterfaceContainer interface_n3_n4;
    interface_n3_n4 = address_n3_n4.Assign(dev_n3_n4);

    // creazione p2p da n3 a n5
    PointToPointHelper p2p_n3_n5;
    p2p_n3_n5.SetDeviceAttribute("DataRate", StringValue("80Mbps"));
    p2p_n3_n5.SetChannelAttribute("Delay", TimeValue(MicroSeconds(5)));

    NetDeviceContainer dev_n3_n5;
    dev_n3_n5 = p2p_n3_n5.Install(l1);

    InternetStackHelper stack_n3_n5;
    stack_n3_n5.Install(l1.Get(1));

    Ipv4AddressHelper address_n3_n5;
    address_n3_n5.SetBase("10.0.2.0", "255.255.255.252");

    Ipv4InterfaceContainer interface_n3_n5;
    interface_n3_n5 = address_n3_n5.Assign(dev_n3_n5);

    // creazione seconda lan a livello di rete
    CsmaHelper csma_2;
    csma_2.SetChannelAttribute("DataRate", StringValue("25Mbps"));
    csma_2.SetChannelAttribute("Delay", TimeValue(MicroSeconds(10)));

    NetDeviceContainer lan_2;
    lan_2 = csma_2.Install(n_lan_2);

    InternetStackHelper stack_lan_2;
    stack_lan_2.Install(n_lan_2);

    Ipv4AddressHelper address_lan_2;
    address_lan_2.SetBase("192.148.2.0", "255.255.255.0");

    Ipv4InterfaceContainer interface_lan_2;
    interface_lan_2 = address_lan_2.Assign(lan_2);

    // creazione p2p da n5 a n6
    PointToPointHelper p2p_n5_n6;
    p2p_n5_n6.SetDeviceAttribute("DataRate", StringValue("80Mbps"));
    p2p_n5_n6.SetChannelAttribute("Delay", TimeValue(MicroSeconds(5)));

    NetDeviceContainer dev_n5_n6;
    dev_n5_n6 = p2p_n5_n6.Install(l2);

    Ipv4AddressHelper address_n5_n6;
    address_n5_n6.SetBase("10.0.3.0", "255.255.255.252");

    Ipv4InterfaceContainer interface_n5_n6;
    interface_n5_n6 = address_n5_n6.Assign(dev_n5_n6);

    // creazione p2p da n4 a n6
    PointToPointHelper p2p_n4_n6;
    p2p_n4_n6.SetDeviceAttribute("DataRate", StringValue("80Mbps"));
    p2p_n4_n6.SetChannelAttribute("Delay", TimeValue(MilliSeconds(5)));

    NetDeviceContainer dev_n4_n6;
    dev_n4_n6 = p2p_n4_n6.Install(l3);

    Ipv4AddressHelper address_n4_n6;
    address_n4_n6.SetBase("10.0.4.0", "255.255.255.252");

    Ipv4InterfaceContainer interface_n4_n6;
    interface_n4_n6 = address_n4_n6.Assign(dev_n4_n6);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    // FINE TOPOLOGIA

    // INIZO CONFIGURAZIONE 0
    if (configuration == 0) {
        // TCP Sink su n0
        uint16_t port_sink_n0 = 2600;
        Address address_sink_n0(InetSocketAddress(interface_lan_1.GetAddress(0), port_sink_n0));
        PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", address_sink_n0);
        ApplicationContainer app = packetSinkHelper.Install(n_lan_1.Get(0));
        app.Start(Seconds(0.0));
        app.Stop(Seconds(20.0));

        //TCP OnOffCLient su n8
        OnOffHelper onOffHelper_n8("ns3::TcpSocketFactory", InetSocketAddress(interface_lan_1.GetAddress(0), port_sink_n0));
        onOffHelper_n8.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper_n8.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper_n8.SetAttribute("PacketSize", UintegerValue(1500));
        ApplicationContainer n8_app;
        n8_app.Add(onOffHelper_n8.Install(n_lan_2.Get(2)));
        n8_app.Start(Seconds(3.0));
        n8_app.Stop(Seconds(15.0));

        csma_1.EnableAscii("task1-0-n0.tr", lan_1.Get(0), true);
        csma_2.EnableAscii("task1-0-n8.tr", lan_2.Get(2), true);
        p2p_n2_n3.EnablePcap("task1-0-n3.pcap", dev_n2_n3.Get(1), false, true);
        csma_2.EnablePcap("task1-0-n6.pcap", lan_2.Get(0), false, true);
    }
    // FINE CONFIGURAZIONE 0

    // INIZIO CONFIGURAZIONE 1
    if (configuration == 1) {
        // TCP Sink su n0
        uint16_t port_sink_n0 = 2600;
        Address address_sink_n0(InetSocketAddress(interface_lan_1.GetAddress(0), port_sink_n0));
        PacketSinkHelper packetSinkHelper_n0("ns3::TcpSocketFactory", address_sink_n0);
        ApplicationContainer hubApp_sink_n0 = packetSinkHelper_n0.Install(n_lan_1.Get(0));
        hubApp_sink_n0.Start(Seconds(0.0));
        hubApp_sink_n0.Stop(Seconds(20.0));

        // TCP Sink su n7
        uint16_t port_sink_n7 = 7777;
        Address address_sink_n7(InetSocketAddress(interface_lan_2.GetAddress(1), port_sink_n7));
        PacketSinkHelper packetSinkHelper_n7("ns3::TcpSocketFactory", address_sink_n7);
        ApplicationContainer hubApp_sink_n7 = packetSinkHelper_n7.Install(n_lan_2.Get(1));
        hubApp_sink_n7.Start(Seconds(0.0));
        hubApp_sink_n7.Stop(Seconds(20.0));

        // TCP OnOffCLient su n8
        OnOffHelper onOffHelper_n8("ns3::TcpSocketFactory", InetSocketAddress(interface_lan_1.GetAddress(0), port_sink_n0));
        onOffHelper_n8.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper_n8.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper_n8.SetAttribute("PacketSize", UintegerValue(2500));
        ApplicationContainer n8_app;
        n8_app.Add(onOffHelper_n8.Install(n_lan_2.Get(2)));
        n8_app.Start(Seconds(5.0));
        n8_app.Stop(Seconds(15.0));

        // TCP OnOffCLient su n1
        OnOffHelper onOffHelper_n1("ns3::TcpSocketFactory", InetSocketAddress(interface_lan_2.GetAddress(1), port_sink_n7));
        onOffHelper_n1.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper_n1.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper_n1.SetAttribute("PacketSize", UintegerValue(5000));
        ApplicationContainer n1_app;
        n1_app.Add(onOffHelper_n1.Install(n_lan_1.Get(1)));
        n1_app.Start(Seconds(2.0));
        n1_app.Stop(Seconds(9.0));

        csma_1.EnableAscii("task1-1-n0.tr", lan_1.Get(0), true);
        csma_1.EnableAscii("task1-1-n1.tr", lan_1.Get(1), true);
        csma_2.EnableAscii("task1-1-n7.tr", lan_1.Get(1), true);
        csma_2.EnableAscii("task1-1-n8.tr", lan_2.Get(2), true);
        p2p_n2_n3.EnablePcap("task1-1-n3.pcap", dev_n2_n3.Get(1), false, true);
        csma_2.EnablePcap("task1-1-n6.pcap", lan_2.Get(0), false, true);
    }
    // FINE CONFIGURAZIONE 1

    // INIZIO CONFIGURAZIONE 2
    if (configuration == 2) {
        // Echo Server su n2
        uint16_t port_n2 = 63;
        UdpEchoServerHelper server_n2(port_n2);
        ApplicationContainer app_n2 = server_n2.Install(n_lan_1.Get(2));
        app_n2.Start(Seconds(0.0));
        app_n2.Stop(Seconds(20.0));

        // Echo Client su n8
        UdpEchoClientHelper client_n8(interface_lan_1.GetAddress(2), port_n2);
        client_n8.SetAttribute("MaxPackets", UintegerValue(5));
        client_n8.SetAttribute("Interval", TimeValue(Seconds(2.0)));
        client_n8.SetAttribute("PacketSize", UintegerValue(2560));
        ApplicationContainer app_n8 = client_n8.Install(n_lan_2.Get(2));

        // MESSAGGIO
        std::string matricola = "0x5b5f60";;
        std::vector<uint8_t> mess(matricola.begin(), matricola.end());
        uint8_t *pt_mess = &mess[0];
        client_n8.SetFill(app_n8.Get(0), pt_mess, matricola.size(), 2560);
        //

        app_n8.Start(Seconds(3.0));
        app_n8.Stop(Seconds(12.0));

        // TCP Sink su n0
        uint16_t port_sink_n0 = 2600;
        Address address_sink_n0(InetSocketAddress(interface_lan_1.GetAddress(0), port_sink_n0));
        PacketSinkHelper packetSinkHelper_n0("ns3::TcpSocketFactory", address_sink_n0);
        ApplicationContainer hubApp_sink_n0 = packetSinkHelper_n0.Install(n_lan_1.Get(0));
        hubApp_sink_n0.Start(Seconds(0.0));
        hubApp_sink_n0.Stop(Seconds(20.0));

        // UDP Sink su n7
        uint16_t port_sink_n7 = 2500;
        Address address_sink_n7(InetSocketAddress(interface_lan_2.GetAddress(1), port_sink_n7));
        PacketSinkHelper packetSinkHelper_n7("ns3::UdpSocketFactory", address_sink_n7);
        ApplicationContainer hubApp_sink_n7 = packetSinkHelper_n7.Install(n_lan_2.Get(1));
        hubApp_sink_n7.Start(Seconds(0.0));
        hubApp_sink_n7.Stop(Seconds(20.0));

        // TCP OnOffCLient su n8
        OnOffHelper onOffHelper_n8("ns3::TcpSocketFactory", InetSocketAddress(interface_lan_1.GetAddress(0), port_sink_n0));
        onOffHelper_n8.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper_n8.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper_n8.SetAttribute("PacketSize", UintegerValue(3000));
        ApplicationContainer app_n8_tcp;
        app_n8_tcp.Add(onOffHelper_n8.Install(n_lan_2.Get(2)));
        app_n8_tcp.Start(Seconds(3.0));
        app_n8_tcp.Stop(Seconds(9.0));

        // UDP OnOffCLient su n8
        OnOffHelper onOffHelper_n8_udp("ns3::UdpSocketFactory", InetSocketAddress(interface_lan_2.GetAddress(1), port_sink_n7));
        onOffHelper_n8_udp.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper_n8_udp.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper_n8_udp.SetAttribute("PacketSize", UintegerValue(3000));
        ApplicationContainer app_n8_udp;
        app_n8_udp.Add(onOffHelper_n8_udp.Install(n_lan_2.Get(2)));
        app_n8_udp.Start(Seconds(5.0));
        app_n8_udp.Stop(Seconds(15.0));

        csma_1.EnableAscii("task1-2-n0.tr", lan_1.Get(0), true);
        csma_1.EnableAscii("task1-2-n2.tr", lan_1.Get(2), true);
        csma_2.EnableAscii("task1-2-n7.tr", lan_2.Get(1), true);
        csma_2.EnableAscii("task1-2-n8.tr", lan_2.Get(2), true);
        p2p_n2_n3.EnablePcap("task1-2-n3.pcap", dev_n2_n3.Get(1), false, true);
        csma_2.EnablePcap("task1-2-n6.pcap", lan_2.Get(0), false, true);
    }
    // FINE CONFIGURAZIONE 2

    //Pcap su tutti i nodi funzionali alla domanda A2
    /*csma_1.EnablePcapAll("task1", false);
    p2p_n2_n3.EnablePcapAll("task1", false);
    p2p_n3_n4.EnablePcapAll("task1", false);
    p2p_n3_n5.EnablePcapAll("task1", false);
    p2p_n4_n6.EnablePcapAll("task1", false);
    p2p_n5_n6.EnablePcapAll("task1", false);
    csma_2.EnablePcapAll("task1", false);*/
    NS_LOG_INFO("Run Simulation.");
    Simulator::Stop(Seconds(20));
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_INFO("Done.");

    return 0;
}