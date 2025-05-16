import socket
import sys
import ssl
import pygame
import chess
import chess.svg
import cairosvg
import io

def receive_message(client_socket):
    response = b""
    while True:
        chunk = client_socket.recv(256)
        if not chunk:
            break
        response += chunk
        if b'\n' in response:
            response = response.split(b'\n', 1)[0]
            break
    return response.decode()


def send_message(sock, message):
    sock.sendall(f"{message}\n".encode())

# Initialize Pygame
pygame.init()

# Set up the screen
WIDTH, HEIGHT = 800, 800
SIZE = 800
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Chess')

# Load SVG image of the chess board
board = chess.Board()
board_svg = chess.svg.board(board=board, size=SIZE).encode('UTF-8')
board_surface = cairosvg.svg2png(bytestring=board_svg)
board_image = pygame.image.load(io.BytesIO(board_surface))

# Positioning of the board image
board_rect = board_image.get_rect(center=(WIDTH // 2, HEIGHT // 2))
orientation = chess.WHITE
running = True

# Helper function to get square position from mouse click
def get_square(pos, orientation):
    x, y = pos
    if(orientation == chess.WHITE):
        col = (x - board_rect.x) // (board_rect.width // 8)
        row = 7 - ((y - board_rect.y) // (board_rect.height // 8))
    else:
        col = 7 - (x - board_rect.x) // (board_rect.width // 8)
        row = ((y - board_rect.y) // (board_rect.height // 8))
    return chess.square(col, row)

# Helper function to highlight possible moves
def draw_highlights(board, square, orientation):
    moves_from_square = [move.to_square for move in board.legal_moves if move.from_square == square]
    king_square = board.king(board.turn)
    in_check = board.is_check()
    
    colors = {}

    # Get the last move played
    if board.move_stack:
        last_move = board.peek()
        from_square = last_move.from_square
        to_square = last_move.to_square
        
        # Fill the squares involved in the last move
        # Yellow color for previous move
        colors[from_square] = "#FFFF00"
        colors[to_square] = "#FFFF00"

    for move in moves_from_square:
        colors[move] = "#0000FF86"  # Fill with blue for legal moves

    if in_check and king_square:
        colors[king_square] = "#FF0000"  # Fill with red for king in check

    return chess.svg.board(
        board,
        orientation = orientation,
        fill=colors,
        size=SIZE 
    )

# Helper function to display new state
def new_board(board, orientation):
    king_square = board.king(board.turn)
    in_check = board.is_check()
    
    colors = {}
    if in_check and king_square:
        colors[king_square] = "#FF0000"  # Fill for king in check
    
    # Get the last move played
    if board.move_stack:
        last_move = board.peek()
        from_square = last_move.from_square
        to_square = last_move.to_square
        
        # Fill the squares involved in the last move
        # Yellow color for previous move
        colors[from_square] = "#FFFF00"
        colors[to_square] = "#FFFF00"

    return chess.svg.board(
        board,
        orientation = orientation,
        fill=colors,
        size=SIZE
    )

#Helper function to help user choose a piece to promote
def promotion_board(board, orientation, selected_square, target_square):
    promotion = board.copy()
    color = board.turn
    #Put a shadow over the whole board
    colors = {}
    for i in range (64):
        colors[i] = "#000000BB"
    #Artificially add pieces to for promotion choice
    square_below_pointer  = 8 if color == chess.WHITE else -8
    promotion_pieces = [chess.QUEEN, chess.KNIGHT, chess.ROOK, chess.BISHOP]
    promotion_buttons = []
    promotion.set_piece_at(selected_square, None)
    for piece in promotion_pieces:
        promotion.set_piece_at(target_square, chess.Piece(piece, color))
        #Enlighten the button square
        colors[target_square] = "#00000000"
        #Remember squares with possible pieces to choose
        promotion_buttons.append(target_square)
        #Go down
        target_square -= square_below_pointer
    #Add empty piece to exit promotion screen
    promotion_pieces.append(None)
    #Create board
    board_svg = chess.svg.board(
        promotion,
        fill=colors,
        orientation = orientation,
        size=SIZE
    ).encode('UTF-8')
    #Display board
    board_surface = cairosvg.svg2png(bytestring=board_svg)
    board_image = pygame.image.load(io.BytesIO(board_surface))
    screen.fill((255, 255, 255))  # Fill the screen with white
    screen.blit(board_image, board_rect)  # Display the chess board image
    pygame.display.flip()  # Update the display
    #Check on which piece the user clicked
    selected_piece = None
    global running  # Declare running as a global variable
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                selected_piece = get_square(pygame.mouse.get_pos(), board.turn)
            elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
                drop = get_square(pygame.mouse.get_pos(), board.turn)
                if(drop == selected_piece):
                    # Find index of drop in promotion_buttons
                    drop_index = next((idx for idx, sq in enumerate(promotion_buttons) if sq == drop), 4)
                    return promotion_pieces[drop_index]

# Helper function to handle pawn promotion
def _move(selected_square, target_square, orientation):
    # Check if it's a pawn
    if board.piece_at(selected_square) and board.piece_at(selected_square).piece_type == chess.PAWN:
        #Check if its moving to the last rank
        last_rank_white = 7  # White's last rank is the 8th (0-7)
        last_rank_black = 0  # Black's last rank is the 1st (0-7)
        target_rank = chess.square_rank(target_square)
        move = chess.Move(selected_square, target_square, promotion=chess.KNIGHT)
        if board.turn == chess.WHITE and target_rank == last_rank_white and move in board.generate_legal_moves():
            piece = promotion_board(board, orientation, selected_square, target_square)
            return chess.Move(selected_square, target_square, promotion=piece)
        elif board.turn == chess.BLACK and target_rank == last_rank_black and move in board.generate_legal_moves():
            piece = promotion_board(board, orientation, selected_square, target_square)
            return chess.Move(selected_square, target_square, promotion=piece)        

    # If no promotion is specified, return the regular move
    return chess.Move(selected_square, target_square)

def animation(info):
    global screen
    font = pygame.font.Font(None, 36)
    text = font.render(info, True, (128, 0, 128))
    text_rect = text.get_rect(center=(WIDTH//2, HEIGHT//2))
    screen.blit(text, text_rect)
    pygame.display.flip()

def move_from_gui():
    global screen
    global board_image
    global board
    global board_svg
    global board_surface
    global orientation
    selected_square = None
    waiting = True
    notation=""
    result=""
    while waiting:
        for event in pygame.event.get():
            #Exiting the game 
            if event.type == pygame.QUIT:
                waiting = False
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                square = get_square(pygame.mouse.get_pos(), board.turn)
                #Check if user clicked a proper square with their own piece 
                if square and square<64 and board.piece_at(square) and board.piece_at(square).color == board.turn and square!=selected_square:
                    selected_square = square
                    # Draw the highlights when a piece is grabbed
                    board_svg = draw_highlights(board, selected_square, orientation)
                #Unpick a piece after double click or after clicking outside the board
                elif square == selected_square or square >= 64 or square == None:
                    selected_square=None
                    #Undo highlights
                    board_svg = new_board(board, orientation).encode('UTF-8')
                board_surface = cairosvg.svg2png(bytestring=board_svg)
                board_image = pygame.image.load(io.BytesIO(board_surface))
            elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
                if(selected_square):
                    target_square = get_square(pygame.mouse.get_pos(), board.turn)
                    move = _move(selected_square, target_square, orientation)
                    #Do move if it is legal
                    if move in board.generate_legal_moves():
                        board.push(move)
                        waiting = False
                        #transform move to text
                        notation = move.uci()
                        result = "U"
                        if board.is_checkmate():
                            result="W" if orientation == chess.WHITE else "B"
                        elif board.is_stalemate():
                            result="D"
                        elif board.is_insufficient_material():
                            result="D"
                        elif board.is_repetition(3):
                            result="D"
                        elif board.is_fifty_moves():
                            result="D"
                    #Probably user wanted to just unpick a piece
                    elif target_square != selected_square:
                        selected_square = None
                    #Remove previous highlights
                    if(target_square != selected_square):
                        board_svg = new_board(board, orientation).encode('UTF-8')
                        board_surface = cairosvg.svg2png(bytestring=board_svg)
                        board_image = pygame.image.load(io.BytesIO(board_surface))

        screen.fill((255, 255, 255))  # Fill the screen with white
        screen.blit(board_image, board_rect)  # Display the chess board image
        pygame.display.flip()  # Update the display
    
    screen.fill((255, 255, 255))  # Fill the screen with white
    screen.blit(board_image, board_rect)  # Display the chess board image
    pygame.display.flip()  # Update the display
    notation = result+notation
    return notation

def move_from_opponent(opponent_move):
    global screen
    global board_image
    global board
    global board_svg
    global board_surface
    global orientation
    #Get raw move
    raw_move = opponent_move[1:]
    move = chess.Move.from_uci(raw_move)
    #Play it
    board.push(move)
    board_svg = new_board(board, orientation).encode('UTF-8')
    board_surface = cairosvg.svg2png(bytestring=board_svg)
    board_image = pygame.image.load(io.BytesIO(board_surface))
    screen.fill((255, 255, 255))  # Fill the screen with white
    screen.blit(board_image, board_rect)  # Display the chess board image
    pygame.display.flip()  # Update the display

def main(host, port):
    global screen
    global board_image
    global board
    global board_svg
    global board_surface
    global orientation
    #Create SSL context with certificate verification disabled
    #Disable verification since server generates its own certificate
    context = ssl.create_default_context()
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE  # Disabling verification
    # Create a socket and wrap it with SSL
    client_socket = socket.create_connection((host, port))
    ssl_socket = context.wrap_socket(client_socket, server_hostname=host)
    animation("Waiting for your opponent...")
    # Find out the color
    color_response = receive_message(ssl_socket)
    color = 0 if color_response[0] == 'W' else 1
    orientation = chess.WHITE if color == 0 else chess.BLACK
    message = 'U'
    # Display the board fromm the appropriate side
    board = chess.Board()
    board_svg = chess.svg.board(board=board, size=SIZE, orientation=orientation).encode('UTF-8')
    board_surface = cairosvg.svg2png(bytestring=board_svg)
    board_image = pygame.image.load(io.BytesIO(board_surface))
    screen.fill((255, 255, 255))  # Fill the screen with white
    screen.blit(board_image, board_rect)  # Display the chess board image
    pygame.display.flip()  # Update the display

    while True:
        #White player
        if color == 0:
            message = move_from_gui()
            send_message(ssl_socket, message)
            if message[0] == "D":
                animation("Draw, nice battle!")
                break
            elif message[0] == "W":
                animation("You won! Congratulations!")
                break
            elif message[0] == "B":
                animation("You lost. Tough luck!")
                break

            server_response = receive_message(ssl_socket)
            move_from_opponent(server_response)
            if server_response[0] == "D":
                animation("Draw, nice battle!")
                break
            elif server_response[0] == "W":
                animation("You won! Congratulations!")
                break
            elif server_response[0] == "B":
                animation("You lost. Tough luck!")
                break
        #Black player
        else:
            server_response = receive_message(ssl_socket)
            move_from_opponent(server_response)
            if server_response[0] == "D":
                animation("Draw, nice battle!")
                break
            elif server_response[0] == "B":
                animation("You won! Congratulations!")
                break
            elif server_response[0] == "W":
                animation("You lost. Tough luck!")
                break

            message = move_from_gui()
            send_message(ssl_socket, message)
            if message[0] == "D":
                animation("Draw, nice battle!")
                break
            elif message[0] == "B":
                animation("You won! Congratulations!")
                break
            elif message[0] == "W":
                animation("You lost. Tough luck!")
                break
    # Close the SSL socket
    ssl_socket.close()
    #Don't close the screen until user wants to
    waiting = True
    while waiting:
        for event in pygame.event.get():
            #Exiting the game 
            if event.type == pygame.QUIT:
                waiting = False
    pygame.quit()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python client.py <hostname> <port>")
    else:
        host = sys.argv[1]
        port = int(sys.argv[2])
        main(host, port)
