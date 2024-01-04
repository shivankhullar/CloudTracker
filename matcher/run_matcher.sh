for sim in m12i_fb_20k m12i_nofb_20k m12i_fb_40k m12i_nofb_40k
do 
    for alpha in {2,5,10}
    do 
        echo $sim, $alpha 
        ./matcher matcher_params.txt n10_alpha$alpha $sim
    done
done
