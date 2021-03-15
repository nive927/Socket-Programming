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
# Creating Nodes 0 to 5
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
#Setting Links based on specifications
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns simplex-link $n2 $n3 0.3Mb 100ms DropTail # link 2-3 is a bottleneck
$ns simplex-link $n3 $n2 0.3Mb 100ms DropTail
$ns duplex-link $n3 $n4 0.5Mb 40ms DropTail
$ns duplex-link $n3 $n5 0.5Mb 40ms DropTail
#Setting Topology - aligning nodes
$ns duplex-link-op $n2 $n0 orient up
$ns duplex-link-op $n2 $n1 orient right-up
$ns simplex-link-op $n2 $n3 orient left
4
$ns simplex-link-op $n3 $n2 orient left
$ns duplex-link-op $n3 $n4 orient up
$ns duplex-link-op $n3 $n5 orient right-up
#Setting Queue Limit to 10
$ns queue-limit $n2 $n3 10
#Setup a TCP connection over 0 and 4 and its flow id, window size, packet size
set tcp [new Agent/TCP/Newreno]
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink/DelAck]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink
$tcp set fid_ 1 # color blue - go to top of script
$tcp set window_ 8000
$tcp set packetSize_ 552
#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP
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
set null [new Agent/Null]
$ns attach-agent $n5 $null
#Connect the traffic source with the traffic sink
$ns connect $udp $null
#Set flow ID, type, packet size, rate and random fields for UDP connection
$udp set fid_ 2
5
#$udp set window_ 8000
$udp set type_ UDP # color red - go to top of script
$udp set packetSize_ 552
$udp set rate_ 0.01mb
$udp set random_ false
#Start and stop the cbr and ftp
$ns at 0.1 "$cbr start"
$ns at 1.0 "$ftp start"
$ns at 4.5 "$ftp stop"
$ns at 5.0 "$cbr stop"
#Call the finish procedure after 5 seconds of simulation time
$ns at 5.0 "finish"
#Run the simulation
$ns run