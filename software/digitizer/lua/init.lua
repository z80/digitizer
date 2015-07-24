
function cb( a, b, c )
  --print( a )
end

ptsCnt = 0
mean   = 0
rms    = 0

function cbFull( a, b, c )
  ptsCnt  = ptsCnt + #a + #b + #c
  --[[if ( #a > 0 ) then
    print( a[1] )
  end]]
end

function main()
  while true do
    voltamp.msleep( 3000 )
    print( "ptsCnt = " .. tostring(ptsCnt) )
    ptsCnt = 0
  end
end


voltamp.dataCallbackRegister( cb )
voltamp.dataCallbackRegisterFull( cbFull )
main()