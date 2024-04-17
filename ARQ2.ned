
network ARQ2
{
    
    @display("bgb=179,108"); 
    types:
        channel C extends ned.DatarateChannel {
           datarate = 10 bps;
            ber = 0.1;
        }
    submodules: 
        source: Source {
            @display("p=40,42");
        } 
        destination: Destination {
            @display("p=124,42");
        }
        connections: 
            source.out --> C --> destination.in; 
            destination.out --> source.in;      
}

simple Source 
{
    parameters: 
        int limit;
        @display ("i=block/source");
    gates: 
        input in; 
        output out; 
}

simple Destination 
{
    
    parameters: 
        @display("i=block/sink"); 
    gates: 
       input in; 
       output out;
}