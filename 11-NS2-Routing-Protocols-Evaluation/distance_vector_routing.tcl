set ns [new Simulator]
$ns color 0 blue
$ns color 1 red
set nf [open out.nam w]
$ns namtrace-all $nf
set tr [open dvp.tr w]
$ns trace-all $tr
proc finish {} {
global nf ns tr
$ns flush-trace
close $tr
exec nam out.nam &
exit 0
}
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
$ns duplex-link $n0 $n8 1Mb 10ms DropTail
$ns duplex-link $n1 $n10 1Mb 10ms DropTail
$ns duplex-link $n0 $n9 1Mb 10ms DropTail
$ns duplex-link $n9 $n11 1Mb 10ms DropTail
$ns duplex-link $n10 $n11 1Mb 10ms DropTail
$ns duplex-link $n11 $n5 1Mb 10ms DropTail
$ns duplex-link $n7 $n6 1Mb 10ms DropTail
$ns duplex-link $n5 $n2 1Mb 10ms DropTail
$ns duplex-link $n5 $n4 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail
3
$ns duplex-link $n11 $n6 1Mb 10ms DropTail
$ns duplex-link-op $n0 $n8 orient left
$ns duplex-link-op $n9 $n0 orient left
$ns duplex-link-op $n10 $n1 orient right
$ns duplex-link-op $n11 $n9 orient left
$ns duplex-link-op $n11 $n10 orient right
$ns duplex-link-op $n11 $n5 orient down
$ns duplex-link-op $n11 $n6 orient up
$ns duplex-link-op $n6 $n7 orient right
$ns duplex-link-op $n5 $n2 orient right
$ns duplex-link-op $n5 $n4 orient left
$ns duplex-link-op $n2 $n3 orient right
set udp [new Agent/UDP]
$ns attach-agent $n0 $udp
set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1
$udp set class_ 0
$udp1 set class_ 1
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns attach-agent $n6 $null
$ns connect $udp $null
$ns connect $udp1 $null
$udp set fid_ 2
$udp set window_ 8000
$udp set packetSize_ 552
$udp1 set fid_ 2
$udp1 set window_ 8000
$udp1 set packetSize_ 552
$ns rtproto LS
$ns rtmodel-at 1.0 down $n11 $n5
$ns rtmodel-at 1.0 down $n7 $n6
4
$ns at 0.0 "$cbr start"
$ns at 0.0 "$cbr1 start"
$ns at 5.0 "finish"
$ns run