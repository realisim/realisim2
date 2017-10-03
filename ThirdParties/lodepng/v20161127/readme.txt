Lodepng was taken from https://github.com/lvandeve/lodepng

Lode version number is found in lodepng.cpp

##########################################################
# Modification a lodepng.cpp
##########################################################

Afin de compiler le fichier avec nos flags de compilations
très sévères (treat warning as error et pedantic)

On a ajouter sur la premiere ligne du fichier

//------------------------------------------------------------------------------
// Since simthetiq code base uses very high warnings, we deactivate them for
// this non propritary file
//
#pragma warning(push, 0)
//------------------------------------------------------------------------------

et sur la derniere ligne du fichier

//------------------------------------------------------------------------------
#pragma warning(pop)
//------------------------------------------------------------------------------

###########################################################

La ligne 4559 à été commenté pour permettre louverture de 
gros fichiers, notamment les images du blue marble kit.

Clairement, c'est pas la meilleur des idées, mais ça fonctionne.

//if(numpixels > 268435455) CERROR_RETURN(state->error, 92);