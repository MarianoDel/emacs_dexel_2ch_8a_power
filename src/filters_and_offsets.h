//-------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### FILTERS_AND_OFFSETS.H #####################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _FILTERS_AND_OFFSETS_H_
#define _FILTERS_AND_OFFSETS_H_


// Exported Types Constants and Macros -----------------------------------------


// Module Exported Functions ---------------------------------------------------
void FiltersAndOffsets_Post_Mapping_SM (volatile unsigned char * ch_dmx_val);
void FiltersAndOffsets_Filters_Reset (void);
void FiltersAndOffsets_Set_Current (unsigned char new_current_int, unsigned char new_current_dec);


#endif    /* _FILTERS_AND_OFFSETS_H_ */

//--- end of file ---//
