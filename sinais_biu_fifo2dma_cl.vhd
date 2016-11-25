----------------

-- Codigo Gerado Pela Ferramenta geraVHD --
-- Autor: Tiago da Silva Curtinhas (2013) --

library ieee;
use ieee.std_logic_1164.all;
ENTITY sinais_biu_fifo2dma_cl IS
PORT(
fain, dackn, ok, cntgt1, rst : OUT std_logic);
END sinais_biu_fifo2dma_cl;

ARCHITECTURE bhv_sinais OF sinais_biu_fifo2dma_cl IS

  constant half_clock_period : time := 10ns;

BEGIN

 sinais: process
  BEGIN

   fain <='0';
   dackn <='0';
   ok <='0';
   cntgt1 <='0';
   rst <='0';
   wait for half_clock_period;
   wait for half_clock_period;
   rst <='1';
   wait for half_clock_period;
   wait for half_clock_period;
   wait for half_clock_period;
end process sinais;

END bhv_sinais;
