NyanCatMake:
	sudo ../bin/lcc -Wa-l -Wl-m -Wl-j -c -o nyan.o nyan.c
	sudo ../bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o music.o music.c
	sudo ../bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o gbt_player.o gbt_player.s
	sudo ../bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o gbt_player_bank1.o gbt_player_bank1.s

	sudo ../bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -Wl-yt1 -Wl-yo4 -Wl-ya0 -o nyan.gb nyan.o music.o gbt_player.o gbt_player_bank1.o
	rm -rf *.o *.lst

	# sudo ../bin/lcc -o nyan.gb nyan.c
	wine ../bgb/bgb.exe nyan.gb