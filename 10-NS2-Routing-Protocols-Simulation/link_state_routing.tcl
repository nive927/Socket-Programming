#Create a simulator object
set ns [new Simulator]
#Open the nam trace file
set nf [open out.nam w]
$ns namtrace-all $nf
$ns color 1 Blue
$ns color 2 Red
#Define a 'finish' procedure
proc finish {} {
global ns nf
$ns flush-trace
#Close the trace file
close $nf
#Execute nam on the trace file
exec nam out.nam &
exit 0
}
# Creating Nodes 0 to 11
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]
#Setting Links based on specifications
$ns duplex-link $n0 $n8 1Mb 10ms DropTail
$ns duplex-link $n1 $n10 1Mb 10ms DropTail
$ns duplex-link $n0 $n9 1Mb 10ms DropTail # link 2-3 is a bottleneck
$ns duplex-link $n9 $n11 1Mb 10ms DropTail
$ns duplex-link $n10 $n11 1Mb 10ms DropTail
$ns duplex-link $n11 $n5 1Mb 10ms DropTail
10
#Setting Topology - aligning nodes
$ns duplex-link-op $n0 $n8 orient left
$ns duplex-link-op $n9 $n0 orient left
$ns duplex-link-op $n10 $n1 orient right
$ns duplex-link-op $n11 $n9 orient left
$ns duplex-link-op $n11 $n10 orient right
$ns duplex-link-op $n11 $n5 orient down
#Setting the node positions for those without links
set Locationx [$n2 set X_ 20]
set Locationy [$n2 set Y_ -25]
set Locationz [$n2 set Z_ 10]
set Locationx [$n3 set X_ 40]
set Locationy [$n3 set Y_ -25]
set Locationz [$n3 set Z_ 10]
set Locationx [$n4 set X_ 60]
set Locationy [$n4 set Y_ -25]
set Locationz [$n4 set Z_ 10]
set Locationx [$n6 set X_ -20]
set Locationy [$n6 set Y_ -25]
set Locationz [$n6 set Z_ 10]
set Locationx [$n7 set X_ -40]
set Locationy [$n7 set Y_ -25]
set Locationz [$n7 set Z_ 10]
#Setting Queue Limit to 10
#$ns queue-limit $n2 $n3 10
#Setup a TCP connection over 0 and 4 and its flow id, window size, packet size
#set tcp [new Agent/TCP/Newreno]
#$ns attach-agent $n0 $tcp
#set sink [new Agent/TCPSink/DelAck]
#$ns attach-agent $n4 $sink
#$ns connect $tcp $sink
#$tcp set fid_ 1 # color blue - go to top of script
#$tcp set window_ 8000
#$tcp set packetSize_ 552
11
#Setup a FTP over TCP connection
#set ftp [new Application/FTP]
#$ftp attach-agent $tcp
#$ftp set type_ FTP
#Create a UDP agent and attach it to node n0
set udp [new Agent/UDP]
$ns attach-agent $n0 $udp
#Create a CBR traffic source and attach it to udp0
set cbr [new Application/Traffic/CBR]
$cbr set type_ CBR
$cbr set packet_size_ 1000
$cbr set rate_ 0.01mb
$cbr set random_ false
$cbr attach-agent $udp
#Create a Null agent (a traffic sink) and attach it to node n5
set null [new Agent/Null]
$ns attach-agent $n5 $null
#Connect the traffic source with the traffic sink
$ns connect $udp $null
#Set flow ID, type, packet size, rate and random fields for UDP connection 0 to 5
$udp set fid_ 1
#$udp set window_ 8000
$udp set type_ UDP # color blue - go to top of script
$udp set packetSize_ 552
$udp set rate_ 0.01mb
$udp set random_ false
#Create a UDP agent and attach it to node n1
set udp [new Agent/UDP]
$ns attach-agent $n1 $udp
#Create a CBR traffic source and attach it to udp1
set cbr [new Application/Traffic/CBR]
$cbr set type_ CBR
$cbr set packet_size_ 1000
$cbr set rate_ 0.01mb
$cbr set random_ false
$cbr attach-agent $udp
#Create a Null agent (a traffic sink) and attach it to node n5
12
set null [new Agent/Null]
$ns attach-agent $n5 $null
#Connect the traffic source with the traffic sink
$ns connect $udp $null
#Set flow ID, type, packet size, rate and random fields for UDP connection 0 to 5
$udp set fid_ 2
#$udp set window_ 8000
$udp set type_ UDP # color red - go to top of script
$udp set packetSize_ 552
$udp set rate_ 0.01mb
$udp set random_ false
#Link state routing
$ns rtproto LS
$ns rtmodel-at 1.0 down $n11 $n5
#$ns rtmodel-at 1.0 down $n7 $n6 #7 - 6 don't have a link
#Start and stop the cbr and ftp
$ns at 0.1 "$cbr start"
#$ns at 1.0 "$ftp start"
#$ns at 4.5 "$ftp stop"
$ns at 5.0 "$cbr stop"
#Call the finish procedure after 5 seconds of simulation time
$ns at 5.0 "finish"
#Run the simulation
$ns run