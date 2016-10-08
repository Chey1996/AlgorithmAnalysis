//Include files
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "cab202_sprites.h"

// ----------------------------------------------------------------
// Global variables containing "long-term" state of program
// ----------------------------------------------------------------


// Tracks in game score
double score = 0;
double gravity = 0.00005;

#define NLEVEL2 3
#define N 4
sprite_id platform[N];
sprite_id platform2[N];
sprite_id platform3[N];
sprite_id platform4[N];
sprite_id platform5[N];
sprite_id safePlatform;
sprite_id player;
sprite_id boss;
int x;
int y;
int max_x;
int max_y;
bool game_over;
int lifes = 3;
int level_number = 1;
double play_duration = 0;
int mins = 0;


// platform Timer
timer_id platform_timer;

//Gravity level 2+ Configuration
#define STEP_CHANGE 1
#define LOOP_STEP 50
#define X_0 5
#define Y_0 20
#define GRAVITY 9
float dx_0, dy_0;
bool shooting;
void refresh_rate_move (void);

//Key Configuration
#define speedslowkey '1'
#define speedsregkey '2'
#define speedfastkey '3'
#define RETRY 'r'
#define LVL 'l'
#define QUIT 'q'

// ----------------------------------------------------------------
// Forward declarations of functions
// ----------------------------------------------------------------

void lifes_remaining();
bool death();
bool no_lives_left();
void setup_zombie2();
bool operation_platform2();
bool process_timer2();
void cleanup_blocks();
void setup2();
void game_movement2();
bool iamdeath2();
bool death2();
bool no_lives_left2();
bool scoreTracker();
void game_movement3();
bool process_timer3();
bool operation_platform3();
void setup3();
void setup3();
bool death3();
bool touching_side();
void setup();
void game_movement();
void cleanup();
void pause_for_exit();
bool process_screen();
bool process_key();
bool process_timer();
void draw_all();
void ui_display();
void draw_turtle();
void player_reset();
void setup_zombie();
bool operation_platform();
void draw_zombie();
void sprite_move();
bool touching();
//

// ----------------------------------------------------------------
// main function
// ----------------------------------------------------------------


void refresh_rate_move (void)
{
player->x += player->dx * LOOP_STEP/1000.0;
player->y += player->dy * LOOP_STEP/1000.0;
player->dx += 0.0;
player->dy += GRAVITY * LOOP_STEP/100.0; //simulating a real-world bollistic motion here (tutor said)

shooting = player->y<screen_height(); //if ball sprite is less than bottom of the bucket, then we want to reset the screen
// if it has gone past the bottom of the bucket, then we go back into shooting mode// if it hits something, then shooting mode is false ??
}

int main( void ) {
	time_t now = time( NULL );
	srand( now );
	setup();
	game_movement();
	cleanup();
	return 0;
}


//Draw and cleanup functions
void cleanup_blocks() {
	for(int i = 0 ; i < N ; i++){
		sprite_destroy(platform[i]);
		platform[i] = NULL;
		sprite_destroy(platform2[i]);
		platform2[i] = NULL;
		sprite_destroy(platform3[i]);
		platform3[i] = NULL;
		sprite_destroy(platform4[i]);
		platform4[i] = NULL;
		sprite_destroy(platform5[i]);
		platform5[i] = NULL;

	}
}

void draw_zombie() {
	for ( int i = 0; i < N; i++ ) {

		sprite_draw(player);
		sprite_draw( safePlatform );
		sprite_draw( platform[i] );
		sprite_draw( platform2[i] );
		sprite_draw( platform3[i] );
		sprite_draw( platform4[i] );
		sprite_draw( platform5[i] );

	}
}

//Key input prcoessing
#include "curses.h"

bool process_key() {
	int key = get_char();

	if ( key == QUIT ) {
		game_over = true;
		return false;
	}


	  double speed = 0.0;
		double speedreg = 0.0;
		double speedfast = 0.0;
	  int original_key = 0;
	  if(key == speedslowkey && level_number == 3){
	    speed = .09 * .25;
			draw_string(23, 23,  "FAST -> SLOW");
			for(int i = 0 ; i < NLEVEL2 ; i++){
				safePlatform->dy=-speed/3;
				player->dy=-speed/3;
				platform[i]->dy=-speed;
				platform2[i]->dy=-speed;
				platform3[i]->dy=-speed;
				platform4[i]->dy=-speed;
				platform5[i]->dy=-speed;
			}
	    if (original_key == speedfastkey) {
	      draw_string(53, 53,  "FAST -> SLOW");
	    }
	    if (original_key == speedsregkey) {
	      draw_string(53, 53,  "ORIGINAL -> SLOW");
	    }
	  }
	  if(key == speedsregkey && level_number == 3){
	    speedreg = .09;
			for(int i = 0 ; i < NLEVEL2 ; i++){
				safePlatform->dy=-speedreg/3;
				player->dy=-speedreg/3;
				platform[i]->dy=-speedreg;
				platform2[i]->dy=-speedreg;
				platform3[i]->dy=-speedreg;
				platform4[i]->dy=-speedreg;
				platform5[i]->dy=-speedreg;
			}
	    if (original_key == speedslowkey) {
	      draw_string(53, 53,  "SLOW -> ORIGINAL");
	    }
	    if (original_key == speedfastkey) {
	      draw_string(53, 53,  "FAST -> ORIGINAL");
	    }
	  }
	  if (key == speedfastkey && level_number == 3){
	    speedfast = .09 * 4;
			for(int i = 0 ; i < NLEVEL2 ; i++){
				safePlatform->dy=-speedfast/3;
				player->dy=-speedfast/3;
				platform[i]->dy=-speedfast;
				platform2[i]->dy=-speedfast;
				platform3[i]->dy=-speedfast;
				platform4[i]->dy=-speedfast;
				platform5[i]->dy=-speedfast;
			}
	    if (original_key == speedslowkey) {
	      draw_string(53, 53, "SLOW -> FAST");
	    }
	    if (original_key == speedsregkey) {
	      draw_string(53, 53,  "ORIGINAL -> FAST");
	    }
	  }

		if (key == speedslowkey) {
			 draw_string(64, 53,  "SLOW");
		 }
		 if (key == speedsregkey) {
			 draw_string(64, 53,  "NORMAL");
		 }
		 if (key == speedfastkey) {
			 draw_string(64, 53, "FAST");
		 }


	// Remember original position
	int x0 = x;
	int y0 = y;

	if(player->x > screen_width()-1){
		player->x--;
		player->dx = 0;
	}
	if(player->x < 0){
		player->x++;
		player->dx = 0;
	}



	if ( key == KEY_LEFT && player->dy < 0) {
		sprite_move(player, -1 , 0 );
	}
	if ( key == KEY_RIGHT  && player->dy < 0 ) {
		sprite_move(player, 1 , 0 );
	}
	//
	// if ( key == KEY_LEFT ) {
	// 	sprite_move(player, -1 , 0 );
	// }
	// if ( key == KEY_RIGHT ) {
	// 	sprite_move(player, 1 , 0 );
	// }


	if( key == LVL){
		level_number = level_number + 1;
	}


	if ( key == KEY_LEFT && level_number == 3 && touching()==true) {
		player->dx = -0.07;
	}
	if ( key == KEY_LEFT && level_number == 2 && touching()==true) {
		player->dx = -0.07;
	}

	if ( key == KEY_RIGHT && level_number == 3 && touching()==true) {
		player->dx = 0.07;
	}
	if ( key == KEY_RIGHT && level_number == 2 && touching()==true) {
		player->dx = 0.07;
	}
	if ( key == KEY_DOWN && level_number == 3 && touching()==true && player->dy < 0) {
		player->dx = 0;
	}
	if ( key == KEY_DOWN && level_number == 2 && touching()==true && player->dy < 0) {
		player->dx = 0;
	}
	if ( key == KEY_UP && level_number == 3  && scoreTracker()==true && player->dy < 0  ) {
		shooting =true;
  player->dy = -90;
		refresh_rate_move();
		}


	if ( key == KEY_UP && level_number == 2  && scoreTracker()==true && player->dy < 0 && player->dx < 0 ) {
		shooting =true;
  player->dy = -90;
		refresh_rate_move();

		// sprite_move(player,player->dx*100,-5);

	}
	if ( key == KEY_UP && level_number == 2  && scoreTracker()==true && player->dy < 0 && player->dx > 0 ) {
		shooting =true;
	player->dy = -90;
		refresh_rate_move();

		// sprite_move(player,player->dx*100,-5);

	}

	int xplatform = safePlatform->x;
	int yplatform = safePlatform->y;

	int xplayer = player->x;
	int yplayer = player->y;


	// Make sure still inside window

	return x0 != x || y0 != y;
}


/*
 *	Processes keyboard timer events to progress game.set
 */
void game_movement() {
	draw_all();
	while ( !game_over ) {
		bool must_redraw = false;
		must_redraw = must_redraw || process_screen();
		must_redraw = must_redraw || process_key();
		must_redraw = must_redraw || process_timer();
		if ( must_redraw ) {
			draw_all();
		}
		while ( lifes == 0 ){
			no_lives_left();
		}
		if( level_number == 2 ){
			setup2();
			game_movement2();
		}

		timer_pause( 30 );
	}
}


void game_movement2() {
	draw_all();
	while ( !game_over ) {
		bool must_redraw = false;
		must_redraw = must_redraw || process_screen();
		must_redraw = must_redraw || process_key();
		must_redraw = must_redraw || process_timer2();
		if ( must_redraw ) {
			draw_all();
		}
		while ( lifes == 0 ){
			no_lives_left2();
		}
		if( level_number == 3){
			setup3();
			game_movement3();
		}
		timer_pause( 30 );
	}
	pause_for_exit();
}

void game_movement3() {
	draw_all();
	while ( !game_over ) {
		bool must_redraw = false;
		must_redraw = must_redraw || process_screen();
		must_redraw = must_redraw || process_key();
		must_redraw = must_redraw || process_timer3();
		if ( must_redraw ) {
			draw_all();
		}
		while ( lifes == 0 ){
			no_lives_left2();
		}
		if( level_number == 4){
			level_number = 1;
			setup();
			game_movement();
		}
		timer_pause( 30 );
	}
	pause_for_exit();
}


/*
 *	Set sup the game.
 */
 void setup() {
 	setup_screen();
 	max_x = screen_width() - 1;
 	max_y = screen_height() - 1;
 	game_over = false;
 	player_reset();
 	setup_zombie();
 }

 void setup2() {
 	setup_screen();
 	max_x = screen_width() - 1;
 	max_y = screen_height() - 1;
 	game_over = false;
 	player_reset();
 	setup_zombie2();
 }
 void setup3() {
 	setup_screen();
 	max_x = screen_width() - 1;
 	max_y = screen_height() - 1;
 	game_over = false;
 	player_reset();
 	setup_zombie2();
 }


 bool touching_side() {
 for(int i = 0 ; i < N ; i++ ){
   int xplayer = (sprite_x(player));
   int yplayer = (sprite_y(player));

    int platblockx = (sprite_x(platform[i]));
  int plat2blockx = (sprite_x(platform2[i]));
  int plat3blockx = (sprite_x(platform3[i]));
  int plat4blockx = (sprite_x(platform4[i]));
  int plat5blockx = (sprite_x(platform5[i]));

   int platblock = (sprite_y(platform[i]));
   int plat2block = (sprite_y(platform2[i]));
   int plat3block = (sprite_y(platform3[i]));
   int plat4block = (sprite_y(platform4[i]));
   int plat5block = (sprite_y(platform5[i]));

  int zomwidth = (platform[i]->width);
  int zom2width = (platform2[i]->width);
  int zom3width = (platform3[i]->width);
  int zom4width = (platform4[i]->width);
  int zom5width = (platform5[i]->width);

  if ( yplayer >= platblock && yplayer <= platblock + 2 ) {
    if (xplayer == (platblockx - 1) || xplayer == (platblockx + zomwidth)) {
    return true;
   }
  }

  if ( yplayer >= plat2block && yplayer <= plat2block + 2) {
    if (xplayer == (plat2blockx - 1) || xplayer == (plat2blockx + zom2width)) {
    return true;
   }
  }

  if ( yplayer >= plat3block && yplayer <= plat3block + 2) {
    if (xplayer == (plat3blockx - 1) || xplayer == (plat3blockx + zom3width)) {
    return true;
   }
  }

  if ( yplayer >= plat4block && yplayer <= plat4block + 2) {
    if (xplayer == (plat4blockx - 1) || xplayer == (plat4blockx + zom4width)) {
    return true;
   }
  }

  if ( yplayer >= plat5block && yplayer <= plat5block + 2) {
    if (xplayer == (plat5blockx - 1) || xplayer == (plat5blockx + zom5width)) {
    return true;
   }
  }

    return false;

}
}

/*
 *	Restore the terminal to normal mode.
 */
void cleanup() {
	cleanup_screen();
}

/*
 *	Turn-based screen processing. Tests for resize events,
 *	returning true if and only if the size has changed.
 */
bool process_screen() {
	int new_max_x = screen_width() - 1;
	int new_max_y = screen_height() - 1;

	bool size_changed = new_max_x != max_x || new_max_y != max_y;

	if ( size_changed ) {
		max_x = new_max_x;
		max_y = new_max_y;
	}

	return size_changed;
}

/*
 *	Process keyboard events, returning true if and only if the
 *	turtle's position has changed.
 */

/*
 *	Process timer events, returning true if and only if the
 *	screen must be updated as the result of time passing.
 */
bool process_timer() {
	bool something_happened = operation_platform();
	return something_happened;
}


bool process_timer2() {
	bool something_happened = operation_platform2();
	return something_happened;
}

bool process_timer3() {
	bool something_happened = operation_platform3();
	return something_happened;
}

/*
 *	Returns the turtle to the centre of the terminal window.
 */
void player_reset() {
	x = 28;
	y = 64;
}

/*
 *	Redraws the screen
 */
void draw_all() {
	clear_screen();
	draw_zombie();
	ui_display();
  lifes_remaining();
	show_screen();
}

/*
 *	Displays a messsage and waits for a keypress.
 */


void lifes_remaining(){
  draw_string( 2, 1, "Lifes Reamaining : " );
	draw_int(22,1, lifes);
}


void pause_for_exit() {
	draw_line( 0, max_y, max_x, max_y, ' ' );
	draw_string( 0, screen_height() - 1, "Press any key to exit...." );
	wait_char();
}


/*
 *	Displays a messsage and waits for a keypress.
 */
void ui_display() {
  draw_line( 0, screen_height()-1, 70,screen_height()-1 , '-' );
  draw_char(0,screen_height()-2,'-');
	draw_string(1,screen_height()-2, "Level:");
	draw_int(10,screen_height()-2, level_number);
	draw_string(1,screen_height()-3, "Score:");
	draw_int(10,screen_height()-3 , score);
	draw_char(0,screen_height()-3,'-');
  draw_char(69,screen_height()-3,'-');
	draw_char(69,screen_height()-2,'-');
  draw_line( 0, screen_height()-4, 70, screen_height()-4, '-' );

  draw_line( 0, 0, 70, 0, '-' );
  draw_char(0,1,'-');
  draw_char(69,1,'-');
  draw_line( 0, 2, 70, 2, '-' );
  draw_string(46,1, "Time:");
  draw_int(62,1, play_duration);
  draw_string(55,1, "Mins");
  draw_int(53,1, mins);
  draw_string(64,1, "Secs");

	if(level_number == 3 ){
		draw_string(30 , screen_height()-3 , "Speeds = Slow(1), Normal(2), Fast(3)");
	}


	draw_string( 2, 68 , "You are on level 1 and your score is :" );
	draw_int(39,68, score);

}


bool scoreTracker(){
if(touching() == true){
	return true;
}
	return false;
}


bool operation_platform2() {


 static char * TENgoodPlatform =
    "=========="
    "=========="
    ;
  static char * TENbadPlatform =
    "xxxxxxxxxx"
    "xxxxxxxxxx"
    ;


 int x_offset[] = {rand() % 12, rand()%(12)+26, rand()%(6)+53};
 int max_y1 = 0;
 int max_y2 = 0;
 int max_y3 = 0;
 int max_y4 = 0;
 int max_y5 = 0;
 int max_y6 = 0;

if (touching_side()==true){
	player->dx = 0;
}

death2();

 if(touching()== true ){
	player->dy = -0.09/3;

}

if(touching() == false  ){
	int holdpos = sprite_y(player);
double y = sprite_dy(player);
gravity = gravity + 0.002;
player->dy = gravity;

}




if(iamdeath2() == true){
	lifes = lifes - 1;
	clear_screen();
	setup2();
	game_movement2();
	draw_all();
}




	//
	// if ( touching()==false && player->dx > 0) {
	// 	player->dx = 0;
	// }
	// if ( touching()==false && player->dx < 0) {
	// 	player->dx = 0;
	// }




 if ( timer_expired( platform_timer ) ) {
	 play_duration = play_duration + 0.0300;
	 if(play_duration > 60){
		 play_duration = 0;
		 mins = mins + 1;
	 }

   time_t now = time( NULL );
   srand( now );

   bool platforms_moved = false;

   for ( int i = 0; i < NLEVEL2; i++ ) {
     double x0 = ( platform[i]->x );
     double y0 = ( platform[i]->y );


     sprite_step( player);
     sprite_step(safePlatform);
     sprite_step( platform[i] );
     sprite_step( platform2[i] );
     sprite_step( platform3[i] );
     sprite_step( platform4[i] );
     sprite_step( platform5[i] );

     if ( platform[i]-> y < -1 && platform5[i]-> y > max_y6) {
       int bitmap = (rand() % (7)+3);
       max_y6 = platform5[i]->y;
 			 platform[i]->y = max_y6 + rand()%(10) + 13;
 			 platform[i]->x = x_offset[i];
			 platform[i]->width = bitmap;
		 }


     if ( platform2[i]->y < -1 && platform[i]->y > max_y1 ) {
        int bitmap = (rand() % (7)+3);
       max_y1 = platform[i]->y;
       platform2[i]->y = max_y1 + rand()%(10) + 13;
       platform2[i]->x = x_offset[i];
			 platform2[i]->width = bitmap;
     }


     if ( platform3[i]->y < -1 && platform2[i]->y > max_y2 ) {
       int bitmap = (rand() % (7)+3);
       max_y2 = platform2[i]->y;
   		 platform3[i]->y = max_y2 + rand()%(10) + 13;
   		 platform3[i]->x = x_offset[i];
			 platform3[i]->width = bitmap;


     }


     if ( platform4[i]->y < -1 && platform3[i]->y > max_y3 ) {
       int bitmap = (rand() % (7)+3);
       max_y3 = platform3[i]->y;
       platform4[i]->y = max_y3 + rand()%(10) +13;
       platform4[i]->x = x_offset[i];
			 platform4[i]->width = bitmap;

     }


     if ( platform5[i]->y < -1 && platform4[i]->y > max_y4 ) {
       int bitmap = (rand() % (7)+3);
       max_y4 = platform4[i]->y;
       platform5[i]->y = max_y4 + rand()%(10) +13;
       platform5[i]->x = x_offset[i];
			 platform5[i]->width = bitmap;
     }



     platforms_moved = platforms_moved || ( platform[i]->x ) != x0 || ( platform[i]->y ) != y0;
   }

	//   bad = rand() % (2);
	//  if(platform2[0]->y > 70 && platform2[1]->y > 70 && platform2[2]->y > 70 ){
	// 	 platform2[bad]->bitmap = TENbadPlatform;
	//  }
	//  bad = rand() % (2);
	//  if(platform3[0]->y > 70 && platform3[1]->y > 70 && platform3[2]->y > 70 ){
	// 	 platform3[bad]->bitmap = TENbadPlatform;
	//  }


   return platforms_moved;
 }

   else {
   return false;

}
}

bool operation_platform3() {


 static char * TENgoodPlatform =
    "=========="
    "=========="
    ;
  static char * TENbadPlatform =
    "**********"
    "**********"
    ;
  // static char * bossright =
  //                                         "<<<<<<<<"
  //                                     "<<<<<<<<<<<<<<<<"
  //                                 "<<<<<<<<<<<<<<<<<<<<<<<"
  //                               "<<<<<<<<<<<<<<<<<<<<<<<<<<<"
  //                              "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
  //                              "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
  //                              "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
  //                              "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
  //                               "<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
  //                                "<<<<<<<<<<<<<<<<<<<<<<<<<"
  //                                     "<<<<<<<<<<<<<<<<"
  //                                        "<<<<<<<<<"
                                         ;
  //
  // static char * bossleft =
  //                                       ">>>>>>>>"
  //                                   ">>>>>>>>>>>>>>>>"
  //                               ">>>>>>>>>>>>>>>>>>>>>>>"
  //                             ">>>>>>>>>>>>>>>>>>>>>>>>>>>"
  //                            ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
  //                            ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
  //                            ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
  //                            ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
  //                             ">>>>>>>>>>>>>>>>>>>>>>>>>>>>"
  //                              ">>>>>>>>>>>>>>>>>>>>>>>>>"
  //                                   ">>>>>>>>>>>>>>>>"
  //                                      ">>>>>>>>>"
  //                                      ;
  // static char * bossup =
  //                                        "^^^^^^^^^"
  //                                    "^^^^^^^^^^^^^^^^"
  //                                 "^^^^^^^^^^^^^^^^^^^^^^^"
  //                               "^^^^^^^^^^^^^^^^^^^^^^^^^^^"
  //                             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
  //                             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
  //                             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
  //                             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
  //                              "^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
  //                                "^^^^^^^^^^^^^^^^^^^^^^^^^"
  //                                    "^^^^^^^^^^^^^^^^"
  //                                       "^^^^^^^^^"
  //                                     ;
  // static char * bossdown =
  //                                    "VVVVVVVVV"
  //                                "VVVVVVVVVVVVVVVV"
  //                             "VVVVVVVVVVVVVVVVVVVVVVV"
  //                           "VVVVVVVVVVVVVVVVVVVVVVVVVVV"
  //                          "VVVVVVVVVVVVVVVVVVVVVVVVVVVVV"
  //                          "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"
  //                          "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"
  //                          "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"
  //                           "VVVVVVVVVVVVVVVVVVVVVVVVVVVV"
  //                            "VVVVVVVVVVVVVVVVVVVVVVVVV"
  //                                 "VVVVVVVVVVVVVVVV"
  //                                    "VVVVVVVVV"
                                     ;
 int x_offset[] = {rand() % 12, rand()%(12)+26, rand()%(6)+53};
 int max_y1 = 0;
 int max_y2 = 0;
 int max_y3 = 0;
 int max_y4 = 0;
 int max_y5 = 0;
 int max_y6 = 0;


 if(iamdeath2() == true){
	 lifes = lifes - 1;
	 clear_screen();
	 setup2();
	 game_movement3();
 }


death3();

if (touching_side()==true){
	player->dx = 0;
}

death2();

 if(touching()== true ){
	player->dy = platform[0]->dy/3;

}

if(touching() == false  ){
	int holdpos = sprite_y(player);
double y = sprite_dy(player);
gravity = gravity + 0.002;
player->dy = gravity;

}




 //}

	//
	// if ( touching()==false && player->dx > 0) {
	// 	player->dx = 0;
	// }
	// if ( touching()==false && player->dx < 0) {
	// 	player->dx = 0;
	// }




 if ( timer_expired( platform_timer ) ) {
	 play_duration = play_duration + 0.0300;
	 if(play_duration > 60){
		 play_duration = 0;
		 mins = mins + 1;
	 }

   time_t now = time( NULL );
   srand( now );

   bool platforms_moved = false;



   for ( int i = 0; i < NLEVEL2; i++ ) {
     double x0 = ( platform[i]->x );
     double y0 = ( platform[i]->y );


     sprite_step( player);
     sprite_step(safePlatform);
     sprite_step( platform[i] );
     sprite_step( platform2[i] );
     sprite_step( platform3[i] );
     sprite_step( platform4[i] );
     sprite_step( platform5[i] );

    //  if (((boss->x) > screen_width()) && ((boss->y) > screen_height())) {
    //    sprite_destroy(boss);
    //  }

     if ( platform[i]-> y < -1 && platform5[i]-> y > max_y6) {
       int bitmap = (rand() % (7)+3);
			    int bad = rand() % (2);
       max_y6 = platform5[i]->y;
 			 platform[i]->y = max_y6 + rand()%(10) + 13;
 			 platform[i]->x = x_offset[i];
			 platform[i]->width = bitmap;

		 }


     if ( platform2[i]->y < -1 && platform[i]->y > max_y1 ) {
        int bitmap = (rand() % (7)+3);
				   int bad = rand() % (2);
       max_y1 = platform[i]->y;
       platform2[i]->y = max_y1 + rand()%(10) + 13;
       platform2[i]->x = x_offset[i];
			 platform2[i]->width = bitmap;

     }


     if ( platform3[i]->y < -1 && platform2[i]->y > max_y2 ) {
       int bitmap = (rand() % (7)+3);
			    int bad = rand() % (2);
       max_y2 = platform2[i]->y;
   		 platform3[i]->y = max_y2 + rand()%(10) + 13;
   		 platform3[i]->x = x_offset[i];
			 platform3[i]->width = bitmap;



     }


     if ( platform4[i]->y < -1 && platform3[i]->y > max_y3 ) {
       int bitmap = (rand() % (7)+3);
			    int bad = rand() % (2);
       max_y3 = platform3[i]->y;
       platform4[i]->y = max_y3 + rand()%(10) +13;
       platform4[i]->x = x_offset[i];
			 platform4[i]->width = bitmap;


     }


     if ( platform5[i]->y < -1 && platform4[i]->y > max_y4 ) {
       int bitmap = (rand() % (7)+3);
			    int bad = rand() % (2);
       max_y4 = platform4[i]->y;
       platform5[i]->y = max_y4 + rand()%(10) +13;
       platform5[i]->x = x_offset[i];
			 platform5[i]->width = bitmap;

     }



     platforms_moved = platforms_moved || ( platform[i]->x ) != x0 || ( platform[i]->y ) != y0;
   }




   return platforms_moved;
 }


   else {
   return false;

}
}




void setup_zombie2(){
{

	   char * TENgoodPlatform =
	     "=========="
	     "=========="
	     ;
	    char * TENbadPlatform =
	     "xxxxxxxxxx"
	     "xxxxxxxxxx"
	     ;
	    static char * playershape =
	     "O"
	     "|"
	     "^"
	   ;




		time_t now = time( NULL );
		srand( now );

		int safePlatformsize = 7;
		player = sprite_create(35,46,1,3, playershape);
		safePlatform = sprite_create(35,49,7,2, TENgoodPlatform);
		safePlatform->width = 7;

      //
      //
			// 					static char * bossright =
			// 						                             "           <<<<<<<<           "
			// 						                             "       <<<<<<<<<<<<<<<<       "
			// 						                             "   <<<<<<<<<<<<<<<<<<<<<<<    "
			// 						                             " <<<<<<<<<<<<<<<<<<<<<<<<<<<  "
			// 						                             "<<<<<<<<<<<<<<<<<<<<<<<<<<<<< "
			// 						                             "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
			// 						                             "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
			// 						                             "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
			// 						                             " <<<<<<<<<<<<<<<<<<<<<<<<<<<< "
			// 						                             "   <<<<<<<<<<<<<<<<<<<<<<<<<  "
			// 						                             "       <<<<<<<<<<<<<<<<       "
			// 						                             "          <<<<<<<<<           "
			// 						                            ;
      //
		  // boss = sprite_create(10,30,30,12, bossright);


		for ( int i = 0; i < N; i++ ) {
		  int x_offset1[] = {rand() % 12, rand()%(12)+26, rand()%(6)+53};

			platform[i] = sprite_create( rand() % screen_width(), 0
				, 7, 2, TENgoodPlatform );

	      int bitmap = rand()%7;
				if (bitmap == 0 ) {
				 platform[i]-> width = 3;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}
				if (bitmap == 1) {
				 platform[i]-> width = 4;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}
				if (bitmap == 2 ) {
				 platform[i]-> width = 5;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}
				if (bitmap == 3 ) {
				 platform[i]-> width = 6;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}
				if (bitmap == 4) {
				 platform[i]-> width = 7;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}
				if (bitmap == 5) {
				 platform[i]-> width = 8;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}
				if (bitmap == 6) {
				 platform[i]-> width = 9;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}
				if (bitmap == 7) {
				 platform[i]-> width = 10;
				 sprite_set_image(platform[i], TENgoodPlatform);
				}


			platform2[i] = sprite_create( rand() % screen_width(), rand()%(24-15)+15+70
					, 7, 2, TENbadPlatform );

					 bitmap = rand()%7;
					if (bitmap == 0 ) {
					 platform2[i]-> width = 3;
					 sprite_set_image(platform2[i], TENgoodPlatform);
					}
					if (bitmap == 1) {
					 platform2[i]-> width = 4;
					 sprite_set_image(platform2[i],TENgoodPlatform);
					}
					if (bitmap == 2 ) {
					 platform2[i]-> width = 5;
					 sprite_set_image(platform2[i], TENgoodPlatform);
					}
					if (bitmap == 3 ) {
					 platform2[i]-> width = 6;
					 sprite_set_image(platform2[i], TENgoodPlatform);
					}
					if (bitmap == 4) {
					 platform2[i]-> width = 7;
					 sprite_set_image(platform2[i], TENgoodPlatform);
					}
					if (bitmap == 5) {
					 platform2[i]-> width = 8;
					 sprite_set_image(platform2[i], TENgoodPlatform);
					}
					if (bitmap == 6) {
					 platform2[i]-> width = 9;
					 sprite_set_image(platform2[i], TENgoodPlatform);
					}
					if (bitmap == 7) {
					 platform2[i]-> width = 10;
					 sprite_set_image(platform2[i], TENgoodPlatform);
					}


			platform3[i] = sprite_create( rand() % screen_width(), rand()%(38-29)+29+70
							, 7, 2, TENbadPlatform );

							 bitmap = rand()%7;
							if (bitmap == 0 ) {
							 platform3[i]-> width = 3;
							 sprite_set_image(platform3[i],TENgoodPlatform );
							}
							else if (bitmap == 1) {
							 platform3[i]-> width = 4;
							 sprite_set_image(platform3[i], TENgoodPlatform);
							}
							else if (bitmap == 2 ) {
							 platform3[i]-> width = 5;
							 sprite_set_image(platform3[i], TENgoodPlatform);
							}
							else if (bitmap == 3 ) {
							 platform3[i]-> width = 6;
							 sprite_set_image(platform3[i], TENgoodPlatform);
							}
							else if (bitmap == 4) {
							 platform3[i]-> width = 7;
							 sprite_set_image(platform3[i], TENgoodPlatform);
							}
							else if (bitmap == 5) {
							 platform3[i]-> width = 8;
							 sprite_set_image(platform3[i], TENgoodPlatform);
							}
							else if (bitmap == 6) {
							 platform3[i]-> width = 9;
							 sprite_set_image(platform3[i], TENgoodPlatform);
							}
							else if (bitmap == 7) {
							 platform3[i]-> width = 10;
							 sprite_set_image(platform3[i], TENgoodPlatform);
							}


			platform4[i] = sprite_create( rand() % screen_width(), rand()%(38-29)+29+70
							, 7, 2, TENbadPlatform );

							 bitmap = rand()%7;
							 if (bitmap == 0 ) {
							 platform4[i]-> width = 3;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}
							else if (bitmap == 1) {
							 platform4[i]-> width = 4;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}
							else if (bitmap == 2 ) {
							 platform4[i]-> width = 5;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}
							else if (bitmap == 3 ) {
							 platform4[i]-> width = 6;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}
							else if (bitmap == 4) {
							 platform4[i]-> width = 7;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}
							else if (bitmap == 5) {
							 platform4[i]-> width = 8;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}
							else if (bitmap == 6) {
							 platform4[i]-> width = 9;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}
							else if (bitmap == 7) {
							 platform4[i]-> width = 10;
							 sprite_set_image(platform4[i], TENgoodPlatform);
							}


			platform5[i] = sprite_create( rand() % screen_width(), rand()%(38-29)+29+70
							, 7, 2, TENbadPlatform );

							 bitmap = rand()%7;
							if (bitmap == 0 ) {
							 platform5[i]-> width = 3;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}
							else if (bitmap == 1) {
							 platform5[i]-> width = 4;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}
							else if (bitmap == 2 ) {
							 platform5[i]-> width = 5;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}
							else if (bitmap == 3 ) {
							 platform5[i]-> width = 6;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}
							else if (bitmap == 4) {
							 platform5[i]-> width = 7;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}
							else if (bitmap == 5) {
							 platform5[i]-> width = 8;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}
							else if (bitmap == 6) {
							 platform5[i]-> width = 9;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}
							else if (bitmap == 7) {
							 platform5[i]-> width = 10;
							 sprite_set_image(platform5[i], TENgoodPlatform);
							}


			platform[i]->x = x_offset1[i];
			platform2[i]->x = x_offset1[i];
			platform3[i]->x = x_offset1[i];
			platform4[i]->x = x_offset1[i];
			platform5[i]->x = x_offset1[i];

			player->dy = -0.09/3;
			safePlatform->dy = -0.09/3;
			platform[i]->dy = -0.09;
			platform2[i]->dy = -0.09;
			platform3[i]->dy = -0.09;
			platform4[i]->dy = -0.09;
			platform5[i]->dy = -0.09;


										platform[i]->y = rand()%(20)+63;
										platform2[i]->y = rand()%(20)+87;
										platform3[i]->y = rand()%(20)+111;
										platform4[i]->y = rand()%(20)+135;
										platform5[i]->y = rand()%(20)+159;


		}

	  int bad = rand() % (2);
		int size = rand() % 7;
		platform[bad]->width = (size + 3);
	  platform[bad]->bitmap = TENbadPlatform;

	  bad = rand()%(3);
		size = rand() % 7;
		platform2[bad]->width = (size + 3);
	  platform2[bad]->bitmap = TENbadPlatform;

	  bad = rand()%(3);
		size = rand() % 7;
		platform3[bad]->width = (size + 3);
	  platform3[bad]->bitmap = TENbadPlatform;

	  bad = rand()%(3);
		size = rand() % 7;
		platform4[bad]->width = (size + 3);
	  platform4[bad]->bitmap = TENbadPlatform;

	  bad = rand()%(3);
		size = rand() % 7;
		platform5[bad]->width = (size + 3);
	  platform5[bad]->bitmap = TENbadPlatform;
	  platform5[bad]->bitmap = TENbadPlatform;

		platform_timer = create_timer( 30 );
	}


}

void setup_zombie() {
	static char * bitmap =
		"======="
		"======="
		 ;

		 static char * notsafe =
			 "xxxxxxx"
			 "xxxxxxx"
				;

				static char * playershape =
					   "O"
					   "|"
             "^"
					 ;

	for ( int i = 0; i < N; i++ ) {
		  int x_offset1[] = {rand() % 10, rand()%(7)+20, rand()%(7)+37, rand()%(10)+54};

int blockModel = rand() % 5;
// int x_offset1[] = { (rand() % (7)), (rand() % (5))+17, (rand() % (4)) +30, (rand() % (14)) +34 };
		player = sprite_create(35,46,1,3, playershape);
		safePlatform = sprite_create(35,49,7,2,bitmap);


		platform[i] = sprite_create( 0, rand()%(20)+63
			, 7, 2, bitmap );
			// if (blockModel >2) {
			// // 	sprite_set_image(platform[i], notsafe);
			// // }
		platform2[i] = sprite_create( 0, rand()%(20)+87
				, 7, 2, bitmap );
				// if (blockModel >2) {
				// 	sprite_set_image(platform2[i], notsafe);
				// }
		platform3[i] = sprite_create( 0, rand()%(20)+111
						, 7, 2, bitmap );
						// if (blockModel >2) {
						// 	sprite_set_image(platform3[i], notsafe);
						// }
		platform4[i] = sprite_create( 0, rand()%(20)+135
									, 7, 2, bitmap );
									// if (blockModel >2) {
									// 	sprite_set_image(platform4[i], notsafe);
									// }
		platform5[i] = sprite_create( 0, rand()%(20)+159
							, 7, 2, bitmap );
							// if (blockModel >2) {
							// 	sprite_set_image(platform5[i], notsafe);
							// }






		platform[i]->x = x_offset1[i];
		platform2[i]->x = x_offset1[i];
		platform3[i]->x = x_offset1[i];
		platform4[i]->x = x_offset1[i];
		platform5[i]->x = x_offset1[i];

		// platform5[i]->x = x_offset1[i];

		player->dy = -0.12/4.000;
		safePlatform->dy = -0.12/4.000;
		platform[i]->dy = -0.12;
		platform2[i]->dy = -0.12;
		platform3[i]->dy = -0.12;
		platform4[i]->dy = -0.12;
		platform5[i]->dy = -0.12;


		// platform5[i]->dy = -0.5;

	}
	int bad = rand()%(4);
  platform[bad]->bitmap = notsafe;
  bad = rand()%(4);
  platform2[bad]->bitmap = notsafe;
  bad = rand()%(4);
  platform3[bad]->bitmap = notsafe;
	bad = rand()%(4);
	platform4[bad]->bitmap = notsafe;
	bad = rand()%(4);
	platform5[bad]->bitmap = notsafe;



	platform_timer = create_timer( 40);
}




void lvlUp(){
	int level_number = 1;
int key = get_char();
if(key = LVL){
	level_number = level_number + 1;
}

}



/*
 * Tries to move the platform; returns true iff the platform moved.
 */
bool no_lives_left(){
	int key = get_char();
if (lifes == 0 ){
clear_screen();
draw_string(10,25,"You have no lifes remaining would you like to continue");
draw_string(15,24,"if so press  r , else press  q  to quit");
if(key == RETRY){
lifes = 3;
score = 0;
mins = 0;
play_duration = 0;

setup();
game_movement();
}
if ( key == QUIT) {
	lifes = -1;
game_over = true;
}
}
}

bool no_lives_left2(){
	int key = get_char();
if (lifes == 0 ){
clear_screen();
draw_string(0,35,"You have no lifes remaining would you like to continue");
draw_string(5,36,"if so press  r , else press  q  to quit");
if(key == RETRY){
	lifes = 3;
	score = 0;
	mins = 0;
	play_duration = 0;
setup2();
game_movement2();
}
if ( key == QUIT) {
	lifes = -1;
cleanup();
exit(0);

}
}
}


bool iamdeath(){
	 int xplayer = (sprite_x(player));
	 int yplayer = (sprite_y(player));
	 int xplatform = safePlatform->x;
	 int yplatform = safePlatform->y;
	 int yvalueplayer = (sprite_y(player));
	 int yvalueblock = (sprite_y(safePlatform));

	static char * notsafe =
		"xxxxxxx"
		"xxxxxxx"
		 ;
for(int i = 0 ; i < N ; i++){
	int platblock = (sprite_y(platform[i]));
	int plat2block = (sprite_y(platform2[i]));
	int plat3block = (sprite_y(platform3[i]));
	int plat4block = (sprite_y(platform4[i]));
	int plat5block = (sprite_y(platform5[i]));


	 if( yvalueplayer == platblock-3 &&  platform[i]->x <= xplayer &&   platform[i]->x+6 >= xplayer && platform[i]->bitmap == notsafe){
		 return true;
	 }

else  if( yvalueplayer == plat2block-3 &&  platform2[i]->x <= xplayer &&   platform2[i]->x+6 >= xplayer && platform2[i]->bitmap == notsafe){
	 return true;
 }

else if( yvalueplayer == plat3block-3 &&  platform3[i]->x <= xplayer &&   platform3[i]->x+6 >= xplayer && platform3[i]->bitmap == notsafe){
	return true;
}

 else if( yvalueplayer == plat4block-3 &&  platform4[i]->x <= xplayer &&   platform4[i]->x+6 >= xplayer && platform4[i]->bitmap == notsafe){
	 return true;
 }

else if( yvalueplayer == plat5block-3 &&  platform5[i]->x <= xplayer &&   platform5[i]->x+6 >= xplayer && platform5[i]->bitmap == notsafe){
	 return true;
 }

}
return false;
}

bool iamdeath2(){
	int xplatform = safePlatform->x;
	int yplatform = safePlatform->y;
	 int xplayer = (sprite_x(player));
	 int yplayer = (sprite_y(player));
	 int yvalueplayer = (sprite_y(player));
	 int yvalueblock = (sprite_y(safePlatform));

	 char * TENbadPlatform =
		"xxxxxxxxxx"
		"xxxxxxxxxx"
		;
for(int i = 0 ; i < N ; i++){
	int platblock = (sprite_y(platform[i]));
	int plat2block = (sprite_y(platform2[i]));
	int plat3block = (sprite_y(platform3[i]));
	int plat4block = (sprite_y(platform4[i]));
	int plat5block = (sprite_y(platform5[i]));


	 if( yvalueplayer == platblock-3 &&  platform[i]->x <= xplayer &&   platform[i]->x+platform[i]->width >= xplayer && platform[i]->bitmap == TENbadPlatform){
		 return true;
	 }

else  if( yvalueplayer == plat2block-3 &&  platform2[i]->x <= xplayer &&   platform2[i]->x+platform2[i]->width  >= xplayer && platform2[i]->bitmap == TENbadPlatform){
	 return true;
 }

else if( yvalueplayer == plat3block-3 &&  platform3[i]->x <= xplayer &&   platform3[i]->x+platform3[i]->width  >= xplayer && platform3[i]->bitmap == TENbadPlatform){
	return true;
}

 else if( yvalueplayer == plat4block-3 &&  platform4[i]->x <= xplayer &&   platform4[i]->x+platform4[i]->width  >= xplayer && platform4[i]->bitmap == TENbadPlatform){
	 return true;
 }

else if( yvalueplayer == plat5block-3 &&  platform5[i]->x <= xplayer &&   platform5[i]->x+platform5[i]->width  >= xplayer && platform5[i]->bitmap == TENbadPlatform){
	 return true;
 }

}
return false;
}




bool touching() {
for(int i = 0 ; i < N ; i++ ){
		int xplatform = safePlatform->x;
		int yplatform = safePlatform->y;

		int xplayer = (sprite_x(player));
		int yplayer = (sprite_y(player));
		int yvalueplayer = (sprite_y(player));
		int yvalueblock = (sprite_y(safePlatform));

		int platblock = (sprite_y(platform[i]));
		int plat2block = (sprite_y(platform2[i]));
		int plat3block = (sprite_y(platform3[i]));
		int plat4block = (sprite_y(platform4[i]));
		int plat5block = (sprite_y(platform5[i]));


			if(  yvalueplayer == yvalueblock-3  &&  xplatform <= xplayer &&   xplatform+safePlatform->width >= xplayer){
				gravity = 0;
					return true;
		}
		if(yvalueplayer == yvalueblock-2  &&  xplatform <= xplayer &&   xplatform+safePlatform->width >= xplayer){
			sprite_move_to(player, player->x , safePlatform->y-3);
			return true;
		}
		/// platform[] ARRAY CODE
		 if( yvalueplayer == platblock-3 &&  platform[i]->x <= xplayer &&   platform[i]->x+platform[i]->width >= xplayer)
		{
			gravity = 0;
			return true;
		}
		if(yvalueplayer == platblock-2  &&  platform[i]->x <= xplayer &&   platform[i]->x+platform[i]->width  >= xplayer){
			sprite_move_to(player, player->x , platform[i]->y-3);
			return true;
		}
		if( yplayer == plat2block-3 &&  platform2[i]->x <= xplayer &&   platform2[i]->x+platform2[i]->width >= xplayer)
		{
			gravity = 0;
			return true;
		}
		if( yplayer == plat2block-2 &&  platform2[i]->x <= xplayer &&   platform2[i]->x+platform2[i]->width >= xplayer)
		{
			sprite_move_to(player, player->x , platform2[i]->y-3);
			return true;
		}
		if( yplayer == plat3block-3 &&  platform3[i]->x <= xplayer &&   platform3[i]->x+platform3[i]->width >= xplayer)
		{
			gravity = 0;
			return true;
		}
	 if( yplayer == plat3block-2 &&  platform3[i]->x <= xplayer &&   platform3[i]->x+platform3[i]->width >= xplayer)
		{
			sprite_move_to(player, player->x , platform3[i]->y-3);
			return true;
			}
		if( yplayer == plat4block-3 &&  platform4[i]->x <= xplayer &&   platform4[i]->x+platform4[i]->width >= xplayer)
		{
			gravity = 0;
			return true;
		}
		 if( yplayer == plat4block-2 &&  platform4[i]->x <= xplayer &&   platform4[i]->x+platform4[i]->width >= xplayer)
		{
			sprite_move_to(player, player->x , platform4[i]->y-3);
			return true;
		}
	if( yplayer == plat5block-3 &&  platform5[i]->x <= xplayer &&   platform5[i]->x+platform5[i]->width>= xplayer)
		{
			gravity = 0;
			return true;
		}
		if( yplayer == plat5block-2 &&  platform5[i]->x <= xplayer &&   platform5[i]->x+platform5[i]->width >= xplayer)
		{
			sprite_move_to(player, player->x , platform5[i]->y-3);
			return true;
		}
	}
}


bool death(){
if(player->y >= screen_height()-5 || player->y <= 3){
	lifes = lifes - 1;
	gravity = 0;
	clear_screen();
	setup();
	game_movement();
	return true;
}
}


bool death2(){
if(player->y >= screen_height()-5 || player->y <= 3){
	lifes = lifes - 1;
	gravity = 0;
	clear_screen();
	setup2();
	game_movement2();
	return true;
}
}

bool death3(){
if(player->y >= screen_height()-5 || player->y <= 3){
	lifes = lifes - 1;
	gravity = 0;
	clear_screen();
	setup2();
	game_movement3();
	return true;
}
}


bool operation_platform() {

	if ( timer_expired( platform_timer ) ) {
		play_duration = play_duration + 0.060;
		if(play_duration > 60){
			play_duration = 0;
			mins = mins + 1;
		}
		static char * notsafe =
			"xxxxxxx"
			"xxxxxxx"
			 ;
			 static char * bitmap =
	 			"======="
	 			"======="
	 			 ;
				 death();



				 if(touching()!=false){
					 score = score + 1;
				 }

				 if(touching()== true ){
			 		player->dy = -0.12/4;

			 	}
					if(touching() == false){

						player->dy = 0.12/4;
					}


				if(iamdeath() == true){
					lifes = lifes - 1;
					clear_screen();
					setup();
					game_movement();
				}

				int score_processes = 0;
				if(touching() == true){
				score_processes == 1;
				if(score_processes == 1){
					score = score + 1;
				}

				}

		bool platforms_moved = false;
			double py = player->dy;

		for ( int i = 0; i < N; i++ ) {
			double x0 = round( platform[i]->x );
			double y0 = round( platform[i]->y );

			int blockModel = rand() % 5;
		  int x_offset1[] = {rand() % 10, rand()%(7)+20, rand()%(7)+37, rand()%(10)+54};



					int xplatform = safePlatform->x;
					int yplatform = safePlatform->y;

					int xplayer = player->x;
					int yplayer = player->y;


				sprite_step(player);
				sprite_step(safePlatform);

			sprite_step( platform[i] );
			sprite_step( platform2[i] );
			sprite_step( platform3[i] );
			sprite_step( platform4[i] );
			sprite_step( platform5[i] );

			// sprite_step( platform5[i] );



			int max_y1 = 0;
			int max_y2 = 0;
			int max_y3 = 0;
			int max_y4 = 0;
			int max_y5 = 0;
			int max_y6 = 0;




if ( platform[i]->y < -1 && platform5[i]->y > max_y6){
			max_y6 = platform5[i]->y;
			platform[i]->y = max_y6 + rand()%(10) + 13;
			platform[i]->x = x_offset1[i];
			if (blockModel >2) {
				sprite_set_image(platform[i], notsafe);
			} else {
				sprite_set_image(platform[i], bitmap);
				}
}
if ( platform2[i]->y < -1 && platform[i]->y > max_y1){
			max_y1 = platform[i]->y;
			platform2[i]->y = max_y1 + rand()%(10) + 13;
			platform2[i]->x = x_offset1[i];
			if (blockModel >3) {
				sprite_set_image(platform2[i], notsafe);
			} else {
				sprite_set_image(platform2[i], bitmap);
			}

}
if ( platform3[i]->y < -1 && platform2[i]->y > max_y2){
		max_y2 = platform2[i]->y;
		platform3[i]->y = max_y2 + rand()%(10) + 13;
		platform3[i]->x = x_offset1[i];
		if (blockModel > 2) {
			sprite_set_image(platform3[i], notsafe);
		} else {
			sprite_set_image(platform3[i], bitmap);
		}
}
if ( platform4[i]->y < -1 && platform3[i]->y > max_y3){
		max_y3 = platform3[i]->y;
		platform4[i]->y = max_y3 + rand()%(10) +13;
		platform4[i]->x = x_offset1[i];
		if (blockModel > 3) {
			sprite_set_image(platform4[i], notsafe);
		} else {
			sprite_set_image(platform4[i], bitmap);
		}


}
if( platform5[i]->y < -1 && platform4[i]->y > max_y4){
		max_y4 = platform4[i]->y;
		platform5[i]->y = max_y4 + rand()%(10) +13;
		platform5[i]->x = x_offset1[i];

		if (blockModel > 2) {
			sprite_set_image(platform5[i], notsafe);
		} else {
			sprite_set_image(platform5[i], bitmap);
		}
}



			platforms_moved = platforms_moved || ( platform[i]->x ) != x0 || ( platform[i]->y ) != y0;
		}

		return platforms_moved;
	}

  	else {
		return false;
	}
}
