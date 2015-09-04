
require( "luafactory" )

factory = luafactory.create()
digit = factory:digitizer()
workV, workI, probeV, probeI = digit:instantValues()
if ( workV ) then
    print( string.format( "workV: %.3f, workI: %.3f, probeV: %.3f, probeI: %.3f", workV, workI, probeV, probeI ) )
end
digit = nil
factory = nil
collectgarbage()

