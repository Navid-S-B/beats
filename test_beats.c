// Assignment 2 20T1 COMP1511: CS bEats
// test_beats.c
//
// This program was written by Navid Bhuiyan
// on 15-04-2020
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Stage 1 tests complete.
// Version 2.0.0: Stage 2 tests semi- completed (wish to put advanced tests).
// Version 3.0.0: Stage 3 tests complete.
// Version 4.0.0: Stage 4 tests complete.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_beats.h"
#include "beats.h"

// Test function for `add_note_to_beat`
int test_add_note_to_beat(void) {

    // Test 1: Rejecting negative inputs.
    Beat test_beat = create_beat();
    if (add_note_to_beat(test_beat, -1, -1) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, -1, 0) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 1, -1) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: Rejecting positive inputs: octaves > 9 and keys > 10
    if (add_note_to_beat(test_beat, 100, 200) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: Rejecting repeat beats/strictly ascending notes.
    if (add_note_to_beat(test_beat, 1, 2) != VALID_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 1, 2) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 4: Rejecting non-ascending octaves
    if (add_note_to_beat(test_beat, 3, 4) != VALID_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 2, 1) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    

    return MEETS_SPEC;
}


// Test function for `count_notes_in_octave`
int test_count_notes_in_octave(void){

    Beat test_beat = create_beat();
    
    // Test 1: Counting Empty Beats
    if (count_notes_in_octave(test_beat, -1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 2: Counting invalid octaves.
    add_note_to_beat(test_beat, 1, 1);
    if (count_notes_in_octave(test_beat, -1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 11) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: Counts valid octaves.
    add_note_to_beat(test_beat, 1, 3);
    if (count_notes_in_octave(test_beat, 1) != 2) {
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}

// Test function for `add_beat_to_track`
int test_add_beat_to_track(void) {
    
    // Create track
    Track test_track = create_track();
    
    // Test 1: Does the function add beats.
    // TODO: Write Test 1
    Beat test_beat_1 = create_beat();
    Beat test_beat_2 = create_beat();
    add_note_to_beat(test_beat_1, 1, 2);
    add_beat_to_track(test_track, test_beat_1);
    add_note_to_beat(test_beat_2, 3, 4);
    add_beat_to_track(test_track, test_beat_2);
    
    if (count_beats_left_in_track(test_track) != 2) {   
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 2: Add beats to track with selection mode on.
    // TODO: Write Test 2
    Beat test_beat_3 = create_beat();
    add_note_to_beat(test_beat_1, 6, 7);
    select_next_beat(test_track);
    add_beat_to_track(test_track, test_beat_3);
    
    if (count_beats_left_in_track(test_track) != 2) {   
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: Add beats to track randomly with selection and without selection.
    // TODO: Write Test 3
    select_next_beat(test_track);
    select_next_beat(test_track); // Selected last beat.
    Beat test_beat_4 = create_beat();
    add_note_to_beat(test_beat_4, 1, 4);
    add_beat_to_track(test_track, test_beat_4);
    select_next_beat(test_track);
    select_next_beat(test_track); // track stopped.
    
    if (count_beats_left_in_track(test_track) != 4) {
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}

// Test function for `remove_selected_beat`
int test_remove_selected_beat(void){

    // Create track
    Track test_track = create_track();
    
    // Test 1: Delete one selected beat, and only selected beat in track_status
    // without error.
    // TODO: Write Test 1
    Beat test_beat_1 = create_beat();
    add_note_to_beat(test_beat_1, 1, 2);
    add_beat_to_track(test_track, test_beat_1);
    select_next_beat(test_track);
    remove_selected_beat(test_track);
    
    if (count_beats_left_in_track(test_track) != 0) {
        return DOES_NOT_MEET_SPEC;   
    }

    // Test 2: Delete selected beat in-between a track and select the beat
    // after the selected beat.
    // TODO: Write Test 2
    Beat test_beat_2 = create_beat();
    add_note_to_beat(test_beat_2, 2, 4);
    add_beat_to_track(test_track, test_beat_2);
    Beat test_beat_3 = create_beat();
    add_beat_to_track(test_track, test_beat_3);
    add_note_to_beat(test_beat_3, 3, 5);
    Beat test_beat_4 = create_beat();
    add_note_to_beat(test_beat_3, 5, 7);
    add_beat_to_track(test_track, test_beat_4);
    select_next_beat(test_track);
    select_next_beat(test_track);
    remove_selected_beat(test_track);
    
    // There should be zero beats left after the last beat.
    if (count_beats_left_in_track(test_track) != 0) {
        return DOES_NOT_MEET_SPEC;   
    }

    // Test 3: Delete last beat and have no selected beat.
    // TODO: Write Test 3
    remove_selected_beat(test_track);
    
    // If no beat is selected, there should be one beat left.
    if (count_beats_left_in_track(test_track) != 1) {
        return DOES_NOT_MEET_SPEC;   
    }

    return MEETS_SPEC;
}

// Test function for `add_musical_note_to_beat`
int test_add_musical_note_to_beat(void){
    
    // Test 1: Can't add same beat.
    // TODO: Write Test 1
    Beat test_beat = create_beat();
    char *test_note_1 = "3A";
    add_musical_note_to_beat(test_beat, test_note_1);
    if (add_musical_note_to_beat(test_beat, "3A") != INVALID_MUSICAL_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: Adds musical valid notes.
    // TODO: Write Test 2
    char *test_note_2 = "3B";
    if (add_musical_note_to_beat(test_beat, test_note_2) != VALID_NOTE) {
        return DOES_NOT_MEET_SPEC;    
    }
    
    // Test 3: Rejects invalid notes.
    // TODO: Write Test 3
    char *test_note_3 = "4B$%$%";
    if (add_musical_note_to_beat(test_beat, test_note_3) != INVALID_MUSICAL_NOTE) {
        return DOES_NOT_MEET_SPEC;    
    }
    
    // Test 4: Adds valid notes with #'s.
    // TODO: Write Test 4
    char *test_note_4 = "5A####";
    if (add_musical_note_to_beat(test_beat, test_note_4) != VALID_NOTE) {
        return DOES_NOT_MEET_SPEC;  
    }

    return MEETS_SPEC;
}

