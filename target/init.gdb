tar extended-remote :2000
define aux-cmd
  set aux_req=1
  set aux_cmd=$arg0
  set aux_data=$arg1
  cont
end
