#for sim in m12i_final_fb_7k m12i_final_nofb_7k m12i_final_fb_57k m12i_final_nofb_57k m12i_final_fb_20k m12i_final_nofb_20k m12i_final_fb_40k m12i_final_nofb_40k
for sim in m12i_final_fb_20k m12i_final_fb_7k m12i_final_nofb_7k
do 
    #for alpha in {2,5,10}
    #do 
        #echo $sim, $alpha 
    ./matcher matcher_params.txt n10_alpha10 $sim
    #done
done
