// Assignment 2 20T1 COMP1511: CS bEats
// beats.c
//
// This program was written by Navid Bhuiyan
// on 14-04-2020
//
/*
PROGRAM DESCRIPTION
    - The program makes music in a simple way. Notes can be added to beats, and
      beats can be easily be built into tracks. It fully integrates Stages 1-4,
      with Stage 5 remaining.
*/
// Version 1.0.0: Assignment released.
// Version 1.0.1: Fix default return value of add_musical_note_to_beat.
// Version 1.0.2: Added all of Stage 1 with passing 19 tests and failing 3.
// Version 1.0.3: Stage 1 Autotesting successful and comments are added.
// Version 1.0.4: Stage 1 functions optimised and comments edited.
// Version 2.0.0: Stage 2 functions optimised and comments edited.
// Version 2.0.1: Note typedef struct addded to ease typing and reading.
// Version 3.0.0: Stage 3 functions complete and comments edited.
// Version 4.0.0: Stage 4 functions complete and comments edited.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.
#include "beats.h"

// Add your own #defines here.
#define TRUE 1
#define FALSE !TRUE
#define IS_NULL -100
#define INVALID_HASH -1

////////////////////////////////////////////////////////////////////////

// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.

struct track {
    // TODO: You will have to add extra fields here in Stage 2.
    struct beat *head;
    // Stores the selected beat in track
    int selected_beat;
    // Stores the track status i.e. if it has been stopped or is playing.
    int track_status;
};

// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.

struct beat {
    // You may choose to add or change fields in this struct.
    struct note *notes;
    struct beat *next;
};

// You don't have to use the provided Note, you are free to
// make your own struct instead.
// If you use the provided Note, you add fields
// to it to store other information.

struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    struct note *next;
};

// Add any other structs you define here.
typedef struct note *Note;

////////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.
int check_valid_octave(int octave);
int check_asc_check_octave(int octave, Note n);
int check_valid_key(int key);
int check_asc_check_key(int octave, int key, Note n);
Beat selected_beat(Track track);
void convert_check_octave(int *octave, int *key, char *musical_notation);
int covert_check_octave(char *musical_notation);
int covert_check_key(char *musical_notation);
int hash_checker_counter(char *musical_notation);
int add_note_ordered(Beat beat, int octave, int key);

// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;

    // You do not need to change this function, unless you change
    // the implementation of struct beat.

    return new_beat;
}

// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add a note to the end of a beat.
int add_note_to_beat(Beat beat, int octave, int key) {
    
    // Check if the octave and key are valid.
    // Refer to functions for further documentation.
    int valid_octave = check_valid_octave(octave);
    int valid_key = check_valid_key(key);
    
    // Filter invalid inputs for octaves and keys
    if (valid_key == FALSE || valid_octave == FALSE) {
    
        if (valid_octave == FALSE) {
            return INVALID_OCTAVE;
        } else {
            return INVALID_KEY;
        }
    
    // If we are starting with an empty beat, then create the start of
    // the note linked list within the beat struct.
    } else if (beat->notes == NULL) {
    
        // Create new note struct by allocating memory and add octave and key.
        Note new_note = malloc(sizeof(struct note));
        new_note->octave = octave;
        new_note->key = key;
        new_note->next = NULL;
    
        // Attach note to beat.
        beat->notes = new_note;       
        
        return VALID_NOTE;    
    
    // If we are not starting with an empty beat, then access the note structs
    // in the beat struct, and go through the linked list.
    } else {
        
        // Create struct addresses for later use in functions and loops
        Note new_note_address = beat->notes;
        Note prev_note_address = beat->notes;
        Note check_octave = beat->notes;
        Note check_key = beat->notes;    
        
        // Get to the NULL note struct via looping, and keep track of the struct
        // before the NULL note struct, to link the new note struct to the 
        // note linked list.
        while (new_note_address != NULL) {
            
            prev_note_address = new_note_address;
            new_note_address = new_note_address->next;
                
        }
        
        // Create new_note struct
        Note new_note = malloc(sizeof(struct note));
       
        // Functions which check the entire linked list to ensure
        // octaves and notes are asc_checkending. Refer to functions 
        // for further documentation.
        // TO NOTE: asc -> ascending.
        int octave_asc_check = check_asc_check_octave(octave, check_octave);
        int key_asc_check = check_asc_check_key(octave, key, check_key);
        
        if (octave_asc_check == TRUE && key_asc_check == TRUE) {
      
            new_note->octave = octave;
            new_note->key = key;
            new_note->next = NULL;
            
            // Attach new_note struct to new_note_address then attach it
            // to the end of the note linked list.
            new_note_address = new_note;    
            prev_note_address->next = new_note_address;
            return VALID_NOTE;
        
        } else {
            
            free(new_note);
            return NOT_HIGHEST_NOTE;    
        
        }
    
    }
         
}

// Print the contents of a beat.
void print_beat(Beat beat) {

    int note_count = 0;
    Note n = beat->notes;
    
    while (n != NULL) {
        // Print the first note with special condition.
        if (note_count == 0) {
            printf("%d %02d", n->octave, n->key);
            note_count = 1;
        // Print the rest of the notes.
        } else {
            printf(" | %d %02d", n->octave, n->key);
        }
        n = n->next;
    }
    
    // Add new line once all of the notes have been printed.
    printf("\n");
}

// Count the number of notes in a beat that are in a given octave.
int count_notes_in_octave(Beat beat, int octave) {
    
    int count_octaves = 0;
    Note n = beat->notes;
    
    while (n != NULL) {
        if (n->octave == octave) {
            count_octaves++;
        }
        n = n->next;
    }
    
    return count_octaves;
}

// Check if octave is valid i.e. between 0 and 9 inclusive.
int check_valid_octave(int octave) {
    if (octave >= 0 && octave <= 9) {
        return TRUE;
    } else {
        return FALSE;    
    }
}

// Checks if the octave inputted is greater or equal then previous octaves.
int check_asc_check_octave(int octave, Note n) {
    int output = TRUE;
    while (n != NULL && output != FALSE) {
        // To check octaves are in ascending order prior to new input octave.
        if (n->octave > octave) { 
            output = FALSE;
        }
        n = n->next;
    }
    return output;
}


// Check if key is valid i.e. between 0 and 11 inclusive.
int check_valid_key(int key) {
    if (key >= 0 && key <= 11) {
        return TRUE;
    } else {
        return FALSE;    
    }
}

// Checks if the octave inputted is greater then previous octaves.
int check_asc_check_key(int octave, int key, Note n) {
    int output = TRUE;
    while (n != NULL && output != FALSE) {
        // To check notes are in ascending order prior to new input key.
        if (n->octave == octave && n->key >= key) {
            output = FALSE;
        }
        n = n->next;
    }
    return output;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    // Note: there is no fprintf in this function, as the
    // Stage 1 autotests call create_track but expect it to return NULL
    // (so this function should do nothing in Stage 1).
    
    // Allocate memory for the track
    Track new_track = malloc(sizeof(struct track));
    
    // Add data to struct properties (refer to struct track for variable 
    // information).
    new_track->head = NULL;
    new_track->selected_beat = IS_NULL;
    new_track->track_status = TRACK_STOPPED; // Don't start with selected beat.    
    
    return new_track;
}

// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {
    
    // Add beat to the head of the track if it is empty
    if (track->head == NULL) {
    
        track->head = beat; 
    
    // If the track is stopped, add beats on top of the beat head.
    } else if (track->track_status == TRACK_STOPPED) {
    
        Beat old_head = track->head;
        track->head = beat;
        (track->head)->next = old_head; 
    
    // If the track is playing. 
    } else {
        
        // Get the current selected struct in linked list 
        Beat current_beat = selected_beat(track);
        
        // If the next beat is NULL, add the bead
        if (current_beat->next == NULL) {
        
            current_beat->next = beat;
        
        // The selected struct is inbetween the linked list.
        } else {
        
            // Store the old next beat
            struct beat *next_beat = current_beat->next;
            
            // Place new beat next to selected beat.
            current_beat->next = beat;
            
            // Make the old next beat the next beat for the selected beat.
            beat->next = next_beat;       
               
        }
        
    }
    
}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) {
    
    if (track->head == NULL) {
    
        return TRACK_STOPPED;    
    
    // Activate track playing if the track is stopped when this function is 
    // called.
    } else if (track->track_status == TRACK_STOPPED) {
        
        track->selected_beat = 1;
        track->track_status = TRACK_PLAYING;        
        return TRACK_PLAYING;
    
    // If the track is currently playing.   
    } else {
        
        // Get selected struct.
        Beat current_beat = selected_beat(track);
        
        // Make track stop if we are at the end of the track.
        if (current_beat->next == NULL) {
            
            track->selected_beat = IS_NULL;
            track->track_status = TRACK_STOPPED;
            return TRACK_STOPPED;
            
        
        // Otherwise, select the next track.
        } else {
        
            track->selected_beat = track->selected_beat + 1;
            return TRACK_PLAYING;
      
        }

    }
    
}

// Print the contents of a track.
void print_track(Track track) {
    
    Beat b = track->head;
    
    int beat_no = 1;
    
    // Loop through track to print beat info
    while (b != NULL) {
        
        // Print arrow for selected beat.
        if (beat_no == track->selected_beat) {
        
            printf(">[%d] ", beat_no);
        
        } else {
        
            printf(" [%d] ", beat_no);
            
        }
        
        // Print the rest of the beat out
        print_beat(b);
        
        // Move to next beat
        b = b->next;
        beat_no++;
         
    }

}

// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {

    Beat b = track->head;    
    
    int counter = 0;
    
    // Count number of beats in track
    while (b != NULL) {
        counter++;            
        b = b->next;
    } 
    
    // Return the number of tracks after selected beat if the track is
    // playing.
    if (track->track_status == TRACK_PLAYING) {
        counter = counter - track->selected_beat;
    }
    
    return counter;

}

// Return current selected beat struct as a pointer
Beat selected_beat(Track track) {
    
    // Get beat head.
    Beat b = track->head;
    
    int beat_position = 1;
    
    // Loop to the beat which is selected.
    while (beat_position != track->selected_beat) {
    
        beat_position++;
        b = b->next;  
    
    }
    
    return b;

}


////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_beat but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than exit).
    
    // Check if beat is null.
    if (beat != NULL) {
    
        // Make note struct to access the notes within the beat.
        Note n = beat->notes;
        
        // Loop through notes and remove their memory
        while (n != NULL) {
            
            Note remove_note = n;
            // Move to next note before removing the current note.
            n = n->next;
            free(remove_note);      
        
        }
        
        // Once all of the notes are freed, free the beat.
        free(beat);
        
    }            
    
}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_track but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than print an error).
    
    // Check if the track is null before the track memory is completely removed.
    if (track != NULL) {
        
        // Make beat struct to make it easier to access the beat list i.e. 
        // track.
        Beat beat = track->head;
        
        // Loop through the beats in the track, and free associated memory.
        while (beat != NULL) {
            
            // Track beat before the loop continues.
            Beat remove_beat = beat;
            beat = beat->next;
            // Use previous function to free a beat.
            free_beat(remove_beat);

        }
        
        // Free the track struct after.
        free(track);

   }
    
}

// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    
    // If the first beat is selected.
    if (track->track_status == TRACK_PLAYING && track->selected_beat == 1) {
        
        Beat free_selected_beat = track->head;    
        
        // Choosing a beat at the beggining of a track.
        if (free_selected_beat->next != NULL) {
            
            // Make the next beat the head beat.
            track->head = free_selected_beat->next;
            free_beat(free_selected_beat);
            return TRACK_PLAYING;     
        
        // Choosing the only beat in the track.
        } else {
        
            track->head = NULL;
            // Reset track struct variables for no other errors with other 
            // functions.
            track->selected_beat = IS_NULL;
            track->track_status = TRACK_STOPPED;            
            free_beat(free_selected_beat);
            return TRACK_STOPPED; 
  
        }
        
    // If any other beat is selected.      
    } else if (track->track_status == TRACK_PLAYING && 
                track->selected_beat != 1) {
      
        Beat free_selected_beat = selected_beat(track);
        
        // Get the beat before the selected beat.
        Beat prev_beat = track->head;
        
        // Loop through track to find the beat before the selected beat.
        while (prev_beat->next != free_selected_beat) {
        
            prev_beat = prev_beat->next;
        
        }
        
        // Link the surrounding beats together to remove the selected beat_no
        // out of the track.
        if (free_selected_beat->next != NULL) {
        
            prev_beat->next = free_selected_beat->next;
            free_beat(free_selected_beat);
            return TRACK_PLAYING;        
        
        // For a beat at the end of the track.   
        } else {
        
            prev_beat->next = NULL;
            // Reset variables as the end of the track is reached. 
            track->selected_beat = IS_NULL;
            track->track_status = TRACK_STOPPED;
            free_beat(free_selected_beat);
            return TRACK_STOPPED;    
                
        }
    
    // If the track is stopped.
    } else {
        return TRACK_STOPPED;
    }
    
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add note to beat, given its 'musical notation'.
int add_musical_note_to_beat(Beat beat, char *musical_notation) {
    
    // Translate musical notation into integers.
    // Functions return the converted key or returns INVALID_KEY or 
    // INVALID_OCTAVE if they are invalid.
    int octave = covert_check_octave(musical_notation);
    int key = covert_check_key(musical_notation);
    
    // Count the number of #'s. Returns INVALID_HASH if anything instead
    // of # is inputted.
    int hash_count = hash_checker_counter(musical_notation);
    
    // If nothing is invalid, adjust the octave and note with the number of
    // hashes.
    if (hash_count != INVALID_HASH && octave != INVALID_OCTAVE && 
        key != INVALID_KEY) {
            
        int overflow_key = key + hash_count;
        int add_to_octave = overflow_key/12;
        octave = octave + add_to_octave;
        key = overflow_key - 12*add_to_octave;
        
        // If octave goes over 9.
        if (check_valid_octave(octave) == FALSE) {
    
            octave = INVALID_OCTAVE;
    
        }
    
    // After adjustment, try and add the note to the beat.
    } if (octave != INVALID_OCTAVE && key != INVALID_KEY && 
            hash_count != INVALID_HASH) {
        
        // Utilise helper function to add beats in ascending order, 
        // and to check for repeat beats.
        int added_or_not = add_note_ordered(beat, octave, key);
        
        // Return if the note being added is valid or invaid.
        // Function returns INVALID_MUSICAL_NOTE & VALID_NOTE.
        return added_or_not;       
    
    } else {
        
        // If key or octave is invalid.
        return INVALID_MUSICAL_NOTE;
    
    }
   
}

// Convert the musical note into a valid octave unless the input is invalid.
int covert_check_octave(char *musical_notation) {

    int octave = atoi(&musical_notation[0]);
    
    if (check_valid_octave(octave) == TRUE) {
        return octave;
    } else {
        return INVALID_OCTAVE;
    }
}

// Convert the musical note into a valid key unless the input is invalid.
int covert_check_key(char *musical_notation) {
    
    // Converter array. Converts letter to designated number.
    int convert_array[7] = {0, 2, 3, 5, 7, 8, 10};
    int converted_key = musical_notation[1];
    
    // Ensure the capital letters are beteeen A to G.
    if (converted_key >= 'A' && converted_key <= 'G') {
        
        converted_key = convert_array[converted_key - 'A'];
        return converted_key;             
    
    // Reject key if a invalid input is given.
    } else {
        return INVALID_KEY;
    }

}

// Count the number of #'s. Return INVALID_HASH if invalid # input occurs.
int hash_checker_counter(char *musical_notation) {
    
    // Start at index 2 as in the char array the #'s start there.
    int index = 2;   
    while (musical_notation[index] != '\0') {
        
        // Break the loop if anything else than a # is detected.
        if (musical_notation[index] != '#') {
            break;
        }
        index++; 
    }
    
    // If the loop broke before '/0', then return INVALID_HASH. 
    if (musical_notation[index] != '\0') {
        return INVALID_HASH;
    } else {
        return index - 2;
    }  

}

// Sorts notes in a beat, and checks if the same note is in the beat.
int add_note_ordered(Beat beat, int octave, int key) {
    
    // Get access the note head in the beat.
    Note note_head = beat->notes;
    
    // Make new note struct.
    Note new_note = malloc(sizeof(struct note));
    new_note->octave = octave;
    new_note->key = key; 
    new_note->next = NULL;
    
    // Check if there are notes in the beat.
    if (note_head != NULL)  {
        
        // Create structs which check the notes, to check if octaves matched
        // the new octave trying to be inputted. prev_note is the note before
        // the check note.
        Note check_note = note_head;
        Note prev_note = NULL;        
        
        // Checks if there is an octave the same as the inputted octave.
        while (check_note->next != NULL && check_note->octave < octave) {
            prev_note = check_note;
            check_note = check_note->next;
        }
        
        // If there are notes with the same octave, loop through notes
        // to see if there is any one note with the same key.
        while (check_note->next != NULL && check_note->octave == octave && 
                check_note->key < key) {
            prev_note = check_note;
            check_note = check_note->next;
        }
        
        // If the starting note has the same octave as the inputted octave.
        if (check_note == note_head) {
        
            // If the input octave is smaller.
            if (octave < check_note->octave) {
            
                beat->notes = new_note;
                new_note->next = check_note;
                return VALID_NOTE;
            
            // If octave is bigger.
            } else if (octave > check_note->octave) {
                
                // If the next note is empty.
                if (check_note->next != NULL) {
                    
                    Note new_next_note = check_note->next;
                    check_note->next = new_note;
                    new_note->next = new_next_note;
                    return VALID_NOTE;
                                   
                } else {
                
                    check_note->next = new_note;
                    return VALID_NOTE;   
                         
                }
            
            // If the key is smaller.  
            } else if (key < check_note->key) {
                
                beat->notes = new_note;
                new_note->next = check_note;
                return VALID_NOTE;
            
            // If the key is larger.
            } else if (key > check_note->key) {
                
                // If the next note is empty.
                if (check_note->next != NULL) {
                    
                    Note new_next_note = check_note->next;
                    check_note->next = new_note;
                    new_note->next = new_next_note;
                    return VALID_NOTE;
                                   
                } else {
                
                    check_note->next = new_note;
                    return VALID_NOTE;   
                         
                }
            
            // If the only note is the same as the input note.
            } else {
                
                free(new_note);
                return INVALID_MUSICAL_NOTE;            
            
            }
        
        // If there is no note with the same input octave within the notes.            
        } else if (check_note->octave != octave) {
            
            // If the input octave is smaller.
            if (octave < check_note->octave)  {
            
                prev_note->next = new_note;
                new_note->next = check_note;
                return VALID_NOTE;
            
            // If the input octave is bigger and is inputted in between the 
            // list.
            } else if (check_note->next != NULL) {
                
                Note new_next_note = check_note->next;
                check_note->next = new_note;
                new_note->next = new_next_note;
                return VALID_NOTE;
            
            // If the note is inputted at the end of the list.
            } else {
            
                check_note->next = new_note;
                return VALID_NOTE;           
            
            }
        
        // Notes with same octave found. 
        } else {
            
            // If the key of the check_note is larger and is in beteeen the 
            // notes.
            if (key > check_note->key && check_note->next == NULL) {
            
                check_note->next = new_note;
                return VALID_NOTE;
            
            // If the key of the check_note is larger and the note is at the 
            // notes.
            } else if (key > check_note->key && check_note->next != NULL) {
                
                Note new_next_note = check_note->next;
                check_note->next = new_note;
                new_note->next = new_next_note;
                return VALID_NOTE;;
            
            // If the key is smaller than the check_note and will
            // be always added previously to the check_note struct.
            } else if (key < check_note->key) {
            
                prev_note->next = new_note;
                new_note->next = check_note;
                return VALID_NOTE;
            
            // The input note cannot be added.   
            } else {
                
                free(new_note);
                return INVALID_MUSICAL_NOTE;
            
            }
        
        }
                
    } else {
    
        beat->notes = new_note;
        return VALID_NOTE;   
    
    }

}


////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

// Cut a range of beats to the end of a track.
void cut_range_to_end(Track track, int range_length) {
    printf("cut_range_to_end not implemented yet.");
    return;
}

// Reverse a list of beats within a range of a track.
int reverse_range(Track track, int range_length) {
    printf("reverse_range not implemented yet.");
    return 0;
}
