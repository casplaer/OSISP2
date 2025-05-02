#!/bin/bash

board=(" " " " " " " " " " " " " " " ")

function print_board() {
    echo "${board[0]} | ${board[1]} | ${board[2]}"
    echo "---------"
    echo "${board[3]} | ${board[4]} | ${board[5]}"
    echo "---------"
    echo "${board[6]} | ${board[7]} | ${board[8]}"
}

function check_win() {
    for i in 0 3 6; do
        if [[ ${board[$i]} != " " && ${board[$i]} == ${board[$((i+1))]} && ${board[$i]} == ${board[$((i+2))]} ]]; then
            return 0
        fi
    done
    for i in 0 1 2; do
        if [[ ${board[$i]} != " " && ${board[$i]} == ${board[$((i+3))]} && ${board[$i]} == ${board[$((i+6))]} ]]; then
            return 0
        fi
    done
    if [[ ${board[0]} != " " && ${board[0]} == ${board[4]} && ${board[0]} == ${board[8]} ]]; then
        return 0
    fi
    if [[ ${board[2]} != " " && ${board[2]} == ${board[4]} && ${board[2]} == ${board[6]} ]]; then
        return 0
    fi
    return 1
}

function valid_move() {
    if [[ ${board[$1]} != " " ]]; then
        echo "Эта клетка уже занята, попробуйте снова."
        return 1
    fi
    return 0
}

turn="X"
while true; do
    print_board
    echo "Игрок $turn, введите номер клетки (1-9):"
    read move
    let "move=$move-1" 

    if [[ $move -ge 0 && $move -le 8 && $(valid_move $move) -eq 0 ]]; then
        board[$move]=$turn
    else
        continue
    fi

    if check_win; then
        print_board
        echo "Игрок $turn выиграл!"
        break
    fi

    if ! [[ " ${board[@]} " =~ " " ]]; then
        print_board
        echo "Ничья!"
        break
    fi

    if [[ $turn == "X" ]]; then
        turn="O"
    else
        turn="X"
    fi
done