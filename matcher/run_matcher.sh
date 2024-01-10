for sim in m12i_fb_7k m12i_nofb_7k m12i_fb_57k m12i_nofb_57k
do 
    for alpha in {2,5,10}
    do 
        echo $sim, $alpha 
        ./matcher matcher_params.txt n10_alpha$alpha $sim
    done
done
