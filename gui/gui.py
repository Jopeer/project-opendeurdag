import pygame
import sys
import serial
import time

# Initialize Pygame
pygame.init()

# Constants
WIDTH = 800
HEIGHT = 600
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
FONT_SIZE = 36
SCORE_CHANGE_ANIMATION_SPEED = 5  # Increase for faster animation
SERIAL_PORT = '/dev/ttyUSB0'  # Modify this according to your system
BAUD_RATE = 9600
PROGRESS_BAR_RADIUS = 100
PROGRESS_BAR_THICKNESS = 10
START_HOLD_TIME = 3  # Time in seconds to hold the button before starting
END_GAME_DELAY = 5  # Delay before the program resets after game ends
WIN_ANIMATION_DURATION = 2  # Duration of win animation in seconds

# Create the screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Score Display")

# Load font
font = pygame.font.SysFont(None, FONT_SIZE)

# Function to display scores
def display_scores(player1_score, player2_score):
    screen.fill(BLACK)
    
    # Render text surfaces for player scores
    player1_text = font.render(f"Player 1: {player1_score}", True, WHITE)
    player2_text = font.render(f"Player 2: {player2_score}", True, WHITE)
    
    # Calculate positions for text
    player1_rect = player1_text.get_rect()
    player1_rect.center = (WIDTH // 4, HEIGHT // 2)
    
    player2_rect = player2_text.get_rect()
    player2_rect.center = (3 * WIDTH // 4, HEIGHT // 2)
    
    # Blit text surfaces onto screen
    screen.blit(player1_text, player1_rect)
    screen.blit(player2_text, player2_rect)

# Function to animate score change
def animate_score_change(player, old_score, new_score):
    change = 1 if new_score > old_score else -1
    while old_score != new_score:
        old_score += change * SCORE_CHANGE_ANIMATION_SPEED
        if (change == 1 and old_score > new_score) or (change == -1 and old_score < new_score):
            old_score = new_score
        display_scores(player1_score, player2_score)
        pygame.time.delay(10)  # Adjust the animation speed
    return new_score

# Function to read serial data
def read_serial():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        while True:
            line = ser.readline().strip().decode()
            if line == "START":
                return True
            elif line == "STOP_PREMATURE":
                return False
            elif line.startswith("P1:"):
                player1_score = int(line.split(":")[1])
            elif line.startswith("P2:"):
                player2_score = int(line.split(":")[1])
            elif line == "STOP_GAME":
                return False
    except serial.SerialException:
        print("Error: Serial port not available.")
        sys.exit()

# Function to draw progress bar
def draw_progress_bar(progress):
    pygame.draw.arc(screen, WHITE, (WIDTH // 2 - PROGRESS_BAR_RADIUS, HEIGHT // 2 - PROGRESS_BAR_RADIUS,
                                    2 * PROGRESS_BAR_RADIUS, 2 * PROGRESS_BAR_RADIUS), -90, -90 + progress, PROGRESS_BAR_THICKNESS)

# Function to display win animation
def display_win_animation(winner):
    start_time = time.time()
    while time.time() - start_time < WIN_ANIMATION_DURATION:
        screen.fill(BLACK)
        winner_text = font.render(f"{winner} WINS!", True, WHITE)
        winner_rect = winner_text.get_rect(center=(WIDTH // 2, HEIGHT // 2))
        screen.blit(winner_text, winner_rect)
        pygame.display.flip()
        pygame.time.delay(10)

# Initial scores
player1_score = 0
player2_score = 0

# Start a thread to read serial data
import threading
serial_thread = threading.Thread(target=read_serial)
serial_thread.start()

# Main game loop
waiting_for_start = True
start_time = None
progress = 0
clock = pygame.time.Clock()
while waiting_for_start:
    screen.fill(BLACK)
    display_scores(player1_score, player2_score)
    
    # Draw progress bar
    draw_progress_bar(progress)
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            waiting_for_start = False
    
    # Check if button is held for 3 seconds
    if start_time is None:
        if pygame.key.get_pressed()[pygame.K_SPACE]:
            start_time = pygame.time.get_ticks() / 1000  # Convert to seconds
    else:
        elapsed_time = pygame.time.get_ticks() / 1000 - start_time
        progress = min(360 * (elapsed_time / START_HOLD_TIME), 360)
        if elapsed_time >= START_HOLD_TIME:
            waiting_for_start = False
        elif not pygame.key.get_pressed()[pygame.K_SPACE]:
            waiting_for_start = False
            progress = 0
    
    pygame.display.flip()
    clock.tick(30)  # Cap FPS to 30
    
# Once the game starts
game_ended = False
while not game_ended:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Check if the game has ended
    game_ended = not read_serial()

# Determine the winner
winner = "Player 1" if player1_score > player2_score else "Player 2" if player2_score > player1_score else "It's a tie!"

# Display win animation
display_win_animation(winner)

# Reset the program after a delay
pygame.time.delay(int(END_GAME_DELAY * 1000))
pygame.quit()
sys.exit()