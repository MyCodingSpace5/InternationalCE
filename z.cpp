#include <climits>
#include <cstddef>
#include <iostream>
#include <new>
#include <string>
using namespace std;
namespace chessEngine{
    
    class Square{
        public:
        int rank;
        int file;
    };
    class Piece{
        public:
            Square position;
            char name;
            int colour;
            int range[2];
            Square possiblemoves[0];
        Piece(Square position, char name, int colour){
            this->position = position;
            this->name = name;
            this->colour = colour;
        }
        int makeMove(Square newpos, int funccall){
            if(newpos.file <= this->range[1] && newpos.rank <= this->range[2] && funccall == 0){
                this->position = newpos;
                possiblemoves[newpos.file + newpos.rank] = newpos;
                return 0;
            }
            if(newpos.file <= this->range[1] && newpos.rank <= this->range[2] && funccall == 1){
                return 0;
            }
            else{
                return 1;
            }
        }
    };
    class chessBoard{
        public:
            string positionname;
            Square board[64];
            Piece whitePieces[0];
            Piece blackPieces[0];
        chessBoard(string loadPosFromFen){
            int k;
            int m;
            for(int i = 0; i <= loadPosFromFen.size(); i++){
                int v = loadPosFromFen[i];
                if(v > 8 || v < 0 && v < 97){
                    whitePieces[i] = new Piece(board[m*8+k], v, 1);
                }
                if(v > 96){
                    blackPieces[i] = new Piece(board[m*8+k], v, 0);
                }
                if(v == 47){
                    m++;
                }
                else{
                    k = loadPosFromFen[i];
                }
            }
        }
        bool vaildMoves(Piece p, Square desiredpos, Piece king){
            if(king.colour == 'w'){
                for(int v = 0; blackPieces[v].colour == 'b'; v++){
                    if(blackPieces[v].makeMove(king.position, 1) == 0){
                        for(int k = 0; whitePieces[v].colour == 'w' k++){
                            if(whitePieces[k].position.file < blackPieces[v].position.file + blackPieces[v].range[0] && whitePieces[k].position.rank < blackPieces[v].position.rank + blackPieces[v].range[1]){
                                break;
                            }
                            else{
                                return false;
                            }
                        }
                    }
                }
                p.makeMove(desiredpos, 0); 
            }
            if(king.colour == 'b'){
                for(int i = 0; whitePieces[i].colour == 'w'; i++){
                    if(whitePieces[i].makeMove(king.position, 1) == 0){
                        for(int v  = 0; blackPieces[v].colour == 'b'; v++){
                            if(blackPieces[v].position.file < whitePieces[i].position.file + whitePieces[i].range[0] && blackPieces[v].position.rank < whitePieces[i].position.rank){
                                break;
                            }
                            else{
                                return false;
                            }
                        }
                    }
                }
                p.makeMove(desiredpos, 0);
            }
            
            return true;
        }
        bool blockVision(Piece p, Piece king, Square desiredpos, Piece attackingPiece){
            if(p.makeMove(desiredpos, 1) == 0 && attackingPiece.range[0] + attackingPiece.position.file > desiredpos.file && attackingPiece.range[1] + attackingPiece.position.rank > desiredpos.rank){
                return true;     
            }
            else{
                return false;
            }
        }
        class Position{
            public:
                string name;
                int ratio[0];
                int depth;
                Square data[0];
            Position(string name, int wins, int losses, int depth, Square data){
                this->name = name;
                this->ratio[0] = wins;
                this->ratio[1] = losses;
                this->data = data;
            }
        };
        struct PieceMove{
            Square move;
            Piece p;
            Piece king;
        };
        Position depth(PieceMove move,  chessBoard originalPosition, chessBoard iteratingposition, int depth, Position blackpos, Piece whiteking, Piece blackking, Position position){
                int legalMoves = 0;
                for(int i = 0; iteratingposition.blackPieces[i].colour == 'b'; i++){
                   for(int m = 0; iteratingposition.board[m].file + iteratingposition.board[m].rank != 0; m++){
                        if(iteratingposition.vaildMoves(iteratingposition.blackPieces[i], iteratingposition.board[m], blackking) == true && blackpos.data[iteratingposition.board[m].file * iteratingposition.board[m].rank].file != iteratingposition.board[m].file){
                            legalMoves++;
                            break;
                        }
                   } 
                }
                if(legalMoves != 1){
                    position.data[move.move.file * move.move.rank] = move.move;
                    position.ratio[move.move.file * move.move.rank]--;
                    originalPosition.depth(move, originalPosition, originalPosition, depth, blackpos, whiteking, blackking, position);
                }
                for(int z = 0; iteratingposition.whitePieces[z].colour == 'w'; z++){
                    for(int k = 0; iteratingposition.board[k].file + iteratingposition.board[k].rank != 0; k++){
                        if(iteratingposition.vaildMoves(iteratingposition.whitePieces[z], iteratingposition.board[k], blackking) == true && position.data[iteratingposition.board[k].file * iteratingposition.board[k].rank].file != iteratingposition.board[k].file){
                            legalMoves++;
                            break;
                        }
                        
                    }
                    
                }
                if(legalMoves != 2){
                    position.data[move.move.file * move.move.rank] = move.move;
                    position.ratio[move.move.file * move.move.rank]--;
                    originalPosition.depth(move, originalPosition, originalPosition, depth--, blackpos, whiteking, blackking, position);
                }
                originalPosition.depth(move, originalPosition, iteratingposition, depth, blackpos, whiteking, blackking, position);
            }
        }
    

