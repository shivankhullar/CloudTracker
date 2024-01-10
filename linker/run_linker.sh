for sim in m12i_fb_7k m12i_nofb_7k m12i_fb_57k m12i_nofb_57k
do 
    for alpha in {2,5,10}
    do 
        echo $sim, $alpha 
        ./linker linker_params0.txt n10_alpha$alpha $sim
        ./linker linker_params3.txt n10_alpha$alpha $sim
	./linker linker_params5.txt n10_alpha$alpha $sim
	./linker linker_params7.txt n10_alpha$alpha $sim
    done
done
