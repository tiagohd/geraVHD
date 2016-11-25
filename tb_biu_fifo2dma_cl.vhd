----------------

-- Codigo Gerado Pela Ferramenta geraVHD --
-- Autor: Tiago da Silva Curtinhas (2013) --

library ieee;
use ieee.std_logic_1164.all;
ENTITY tb_biu_fifo2dma_cl IS
END tb_biu_fifo2dma_cl;

ARCHITECTURE tb OF tb_biu_fifo2dma_cl IS

signal fain, dackn, ok, cntgt1, rst : std_logic;
signal fgr : std_logic;
signal dreq, frout, ss00, ss01 : std_logic;

-- declaração de components
component biu_fifo2dma_cl is
PORT (
fain, dackn, ok, cntgt1, rst : IN std_logic;
fgr : OUT std_logic;
dreq, frout, ss00, ss01 : OUT std_logic);
end component;

-- declaração de components
component sinais_biu_fifo2dma_cl is
PORT (fain, dackn, ok, cntgt1, rst : OUT std_logic);end component;

BEGIN

-- port map declaration for
 comp : biu_fifo2dma_cl
  PORT MAP (
fain=>fain, dackn=>dackn, ok=>ok, cntgt1=>cntgt1, rst=>rst,
fgr=>fgr, dreq=>dreq, frout=>frout, ss00=>ss00, ss01=>ss01);

-- port map declaration for
suv : sinais_biu_fifo2dma_cl
PORT MAP (
fain=>fain, dackn=>dackn, ok=>ok, cntgt1=>cntgt1, rst=>rst);

END architecture tb;
