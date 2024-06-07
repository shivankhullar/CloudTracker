
#for sim in m12i_final_fb_7k m12i_final_nofb_7k m12i_final_fb_20k m12i_final_nofb_20k m12i_final_fb_40k m12i_final_nofb_40k m12i_final_fb_57k m12i_final_nofb_57k
#do 
#    for alpha in {2,5,10}
#    do 
#        echo $sim, $alpha 
#        ./linker linker_params0.txt n10_alpha$alpha $sim
#	./linker linker_params3.txt n10_alpha$alpha $sim
#	./linker linker_params5.txt n10_alpha$alpha $sim
#	./linker linker_params7.txt n10_alpha$alpha $sim
        #./linker linker_params3.txt n10_alpha$alpha $sim
	#./linker linker_params5.txt n10_alpha$alpha $sim
	#./linker linker_params7.txt n10_alpha$alpha $sim
#    done
#done

#sim=m12i_final_fb_20k
#alpha=10
#./linker linker_params0.txt n10_alpha$alpha $sim
#./linker linker_params3.txt n10_alpha$alpha $sim
#./linker linker_params5.txt n10_alpha$alpha $sim
#./linker linker_params7.txt n10_alpha$alpha $sim

#sim=m12i_final_fb_7k
#alpha=10
#./linker linker_params0.txt n10_alpha$alpha $sim
#./linker linker_params3.txt n10_alpha$alpha $sim
#./linker linker_params5.txt n10_alpha$alpha $sim
#./linker linker_params7.txt n10_alpha$alpha $sim

sim=m12i_final_nofb_7k
alpha=5
./linker linker_params0.txt n10_alpha$alpha $sim
./linker linker_params3.txt n10_alpha$alpha $sim
./linker linker_params5.txt n10_alpha$alpha $sim
./linker linker_params7.txt n10_alpha$alpha $sim
