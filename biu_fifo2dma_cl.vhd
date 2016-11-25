----------------

-- Codigo Gerado Pela Ferramenta geraVHD --
-- Autor: Tiago da Silva Curtinhas (2013) --

library ieee;
use ieee.std_logic_1164.all;
LIBRARY altera_mf;
USE altera_mf.altera_mf_components.all;
ENTITY biu_fifo2dma_cl IS
PORT(
		fain, dackn, ok, cntgt1, rst : IN std_logic;
		fgr : OUT std_logic;
		dreq, frout, ss00, ss01 : OUT std_logic);
END biu_fifo2dma_cl;

ARCHITECTURE bhv OF biu_fifo2dma_cl IS

  component lcell
   port ( a_in : in std_logic;  a_out : out std_logic);
  end component;


 SIGNAL CK, s_fgr : std_logic;
 SIGNAL s_dreq, s_frout, s_ss00, s_ss01 : std_logic;
 SIGNAL d_fgr : std_logic;
 SIGNAL d_dreq, d_frout, d_ss00, d_ss01 : std_logic;

BEGIN

  buff: lcell port map (s_fgr, CK);
  fgr <= s_fgr;
  dreq <= s_dreq;
  frout <= s_frout;
  ss00 <= s_ss00;
  ss01 <= s_ss01;


 abc: process (rst, CK)
 BEGIN

  if rst = '0' then

   s_dreq <= '0';
   s_frout <= '0';
   s_ss00 <= '0';
   s_ss01 <= '0';

  elsif rising_edge(CK) then

   s_fgr <= d_fgr;
   s_dreq <= d_dreq;
   s_frout <= d_frout;
   s_ss00 <= d_ss00;
   s_ss01 <= d_ss01;

  end if;
 END process abc;
   s_dreq <=( fain AND  dackn);
   s_frout <=( ok AND NOT(fain) AND  dackn);
   s_ss00 <=((NOT(s_frout) AND  s_ss00) OR (NOT(dackn) AND NOT(cntgt1) AND NOT(s_ss01)) OR (NOT(fain) AND  s_ss00));
   s_ss01 <=((NOT(s_frout) AND NOT(s_ss00) AND  s_ss01) OR (NOT(fain) AND NOT(s_ss00) AND  s_ss01) OR (NOT(dackn) AND  cntgt1 AND NOT(s_ss00)) OR (NOT(ok)));
   fgr <=(( ok AND NOT(fain) AND  dackn AND NOT(s_frout) AND  s_ss01) OR (NOT(ok) AND NOT(fain) AND  dackn AND NOT(s_ss01)) OR ( fain AND NOT(s_dreq) AND  s_frout AND  s_ss00) OR ( fain AND NOT(s_dreq) AND  s_frout AND  s_ss01) OR (NOT(dackn) AND NOT(s_ss00) AND NOT(s_ss01)));

END bhv;
