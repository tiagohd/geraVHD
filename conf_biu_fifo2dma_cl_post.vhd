----------------
-- Codigo Gerado Pela Ferramenta geraVHD --
-- Autor: Tiago da Silva Curtinhas (2013) --

configuration conf_biu_fifo2dma_cl_post of tb_biu_fifo2dma_cl is
 for tb
  for comp : biu_fifo2dma_cl use entity work.biu_fifo2dma_cl(structure);
  end for;
  for suv : sinais_biu_fifo2dma_cl use entity work.sinais_biu_fifo2dma_cl(bhv_sinais);
  end for;
 end for;
end conf_biu_fifo2dma_cl_post;
