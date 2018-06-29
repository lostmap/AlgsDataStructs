//
//  Calculator.c
//  Калькулятор со скобками
//
//  Time limit:  14 s
//  Memory limit:  64 M
//  Требуется написать программу, которая способна вычислять арифметические выражения.
//  Выражения могут содержать:
//  1) знаки операций '+', '-', '/', '*'
//  2) Скобки '(', ')'
//  3) Целые и вещественные числа, в нотации '123', '123.345', все операции должны быть вещественны,
//  а результаты выведены с точностю до двух знаков после запятой в том числе целые '2.00'
//  4) необходимо учитывать приоритеты операций, и возможность унарного минуса, пробелы ничего не значат
//  5) Если в выражении встретилась ошибка требуется вывести в стандартный поток вывода "[error]" (без кавычек)
//
//  ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите return -1, exit(1) и т.п.
//  Даже если обнаружилась какая-то ошибка, все равно необходимо вернуть 0! (и напечатать [error] в stdout).
//
//  Copyright © 2018 lostmap. All rights reserved.
//  TODO: fix unary op & spaces

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

struct String {
  char* str_;
  size_t size_;
};

void free_string(struct String *del_string) {
  if (del_string == NULL)
    return;
  free(del_string->str_);
  del_string->str_ = NULL;
  free(del_string);
  del_string = NULL;
}

struct String *create_string(const char* str, size_t size) {
  if (str == NULL) {
    return NULL;
  }
  struct String *new_string = malloc(sizeof(struct String));
  if (new_string == NULL) {  // if malloc fails
    return NULL;
  }
  new_string->size_ = size;
  new_string->str_ = malloc(size * sizeof(char));
  if (new_string->str_ == NULL) {
    free_string(new_string);
    return NULL;
  }
  for (size_t i = 0; i < size - 1; ++i) {
    new_string->str_[i] = str[i];
  }
  new_string->str_[size - 1] = '\0';
  return new_string;
}


struct Stack {
  struct String **buffer_;
  size_t buffer_capacity_;
  size_t size_;
  size_t MAX_;
  int32_t top_;
};
void free_stack(struct Stack *del_stack) {
  if (del_stack == NULL)
    return;
  for (size_t i = 0; i < del_stack->buffer_capacity_; i++) {
    if (i < del_stack->MAX_) {
      free_string(del_stack->buffer_[i]);
    } else {
      free(del_stack->buffer_[i]);
    }
  }
  free(del_stack->buffer_);
  del_stack->buffer_ = NULL;
  free(del_stack);
  del_stack = NULL;
}
struct Stack *create_stack(void) {
  struct Stack *new_stack = malloc(sizeof(struct Stack));
  if (new_stack == NULL) {  // if malloc fails
    return NULL;
  }
  new_stack->buffer_ = NULL;
  new_stack->buffer_capacity_ = 0;
  new_stack->size_ = 0;
  new_stack->MAX_ = 0;
  new_stack->top_ = -1;
  return new_stack;
}

struct Stack *stack_extend(struct Stack *stack) {
  if (stack == NULL) {
    return NULL;
  }
  size_t new_buffer_capacity = 1;
  if (stack->buffer_capacity_ != 0) {
    new_buffer_capacity = stack->buffer_capacity_ * 2;
  }
  struct String **new_buffer = realloc(stack->buffer_, new_buffer_capacity * sizeof(struct String*));
  if (new_buffer == NULL) {
    free_stack(stack);
    return NULL;
  }
  for (size_t i = stack->buffer_capacity_; i < new_buffer_capacity; ++i) {
    new_buffer[i] = NULL;
  }
  stack->buffer_ = new_buffer;
  stack->buffer_capacity_ = new_buffer_capacity;
  return stack;
}
struct Stack *push(struct Stack *stack, char *str) {
  if (stack == NULL || str == NULL) {
    return NULL;
  }
  size_t size = strlen(str);
  if (size == 0) {
    return NULL;
  }
  if (stack->top_ + 1 == stack->buffer_capacity_) {
    stack = stack_extend(stack);
    if (stack == NULL) {
      return NULL;
    }
  }
  stack->top_++;
  free_string(stack->buffer_[stack->top_]);
  stack->buffer_[stack->top_] = NULL;
  stack->buffer_[stack->top_] = create_string(str, size + 1);
  if (stack->buffer_[stack->top_] == NULL) {
    return NULL;
  }
  stack->size_++;
  if (stack->size_ > stack->MAX_)
    stack->MAX_ = stack->size_;
  return stack;
}
struct Stack *push_char(struct Stack *stack, char ch) {
  if (stack == NULL) {
    return NULL;
  }
  if (stack->top_ + 1 == stack->buffer_capacity_) {
    stack = stack_extend(stack);
    if (stack == NULL) {
      return NULL;
    }
  }
  stack->top_++;
  free_string(stack->buffer_[stack->top_]);
  stack->buffer_[stack->top_] = NULL;
  stack->buffer_[stack->top_] = create_string(&ch, 2);
  if (stack->buffer_[stack->top_] == NULL) {
    return NULL;
  }
  stack->size_++;
  if (stack->size_ > stack->MAX_)
    stack->MAX_ = stack->size_;
  return stack;
}
struct Stack *pop(struct Stack *stack, char **str) {
  if (stack == NULL || str == NULL) {
    return NULL;
  }
  if (stack->top_ == -1) {
    return NULL;
  }
  *str = stack->buffer_[stack->top_]->str_;
  if (str == NULL) {
    return NULL;
  }
  stack->top_--;
  stack->size_--;
  return stack;
}
struct Stack *isEmpty(struct Stack *stack, bool *flag) {
  if (stack == NULL || flag == NULL) {
    return NULL;
  }
  if (stack->top_ == -1) {
    *flag = true;
  } else {
    *flag = false;
  }
  return stack;
}
void print(struct Stack *stack) {
  printf("<|");
  for (size_t i = 0; i < stack->size_; i++) {
    printf("%s|", stack->buffer_[i]->str_);
  }
  printf(">\n");
}
size_t countDigits(double num, size_t prec) {  // digits + . + precision
  size_t count = 0;
  if (num <= 0) {
    count = 1;
  }
  int32_t to = (int32_t)num;
  while (to != 0) {
    to = to / 10;
    count++;
  }
  return count + prec + 1;
}
struct Stack *operand(struct Stack *tokens, const char *str,
                      size_t start, size_t end,
                      bool *found, bool *stop) {
  if (tokens == NULL || str == NULL || found == NULL || stop == NULL) {
    return NULL;
  }
  
  if (*found) {
    *found = false;
  } else {
    *stop = true;
  }
  if (*stop == false && *found == false) {
    
    char* buff = malloc((end - start + 1) * sizeof(char));
    if (buff == NULL) {
      return NULL;
    }
    for (size_t i = start; i < end; ++i) {
      buff[i - start] = str[i];
    }
    buff[end - start] = '\0';
    if (push(tokens, buff) == NULL) {  // if push fails
      free(buff);
      return NULL;
    }
    free(buff);
    buff = NULL;
  }
  return tokens;
}
enum {
  DIGIT,
  L_BRACKET,
  R_BRACKET = L_BRACKET,
  PLUS,
  MINUS = PLUS,
  MULTI,
  DIVIDE = MULTI,
};

int32_t priority(char symbol) {
  if(symbol == '(')
    return L_BRACKET;
  if(symbol == ')')
    return R_BRACKET;
  if(symbol == '+')
    return PLUS;
  if(symbol == '-')
    return MINUS;
  if(symbol == '*')
    return MULTI;
  if(symbol == '/')
    return DIVIDE;
  return DIGIT;
}
struct Stack *doMath(struct Stack *revers, const char *tok,
                     const char *op1, const char *op2, double *out) {
  
  if (revers == NULL || tok == NULL || op1 == NULL || op2 == NULL || out == NULL) {
    return NULL;
  }
  if (tok[0] == '*') {
    *out = atof(op1) * atof(op2);
    return revers;
    
  } else if ((tok[0] == '/')) {
    *out = atof(op1) / atof(op2);
    return revers;
    
  } else if ((tok[0] == '+')) {
    *out = atof(op1) + atof(op2);
    return revers;
    
  }  else if ((tok[0] == '-')) {
    *out = atof(op1) - atof(op2);
    return revers;
  }
  return NULL;
}
char* getString(char *buffer, bool* is_eof) {
  if (buffer != NULL) {
    return NULL;
  }
  if (is_eof == NULL) {
    return NULL;
  }
  size_t size = 0;
  size_t capacity = 1;
  char c = '\0';
  int32_t result = 0;
  do {
    result = scanf("%c", &c);
    if (result == EOF) {  // EOF
      *is_eof = true;
      return buffer;
    } else {
      if (result != 1) {  // symbol input error
        return NULL;
      }
    }
    if (size + 1 >= capacity) {
      size_t new_capacity = capacity * 2;
      char* new_buffer = realloc(buffer, (new_capacity + 1) * sizeof(char));
      if (new_buffer == NULL) {  // if realloc fails
        return NULL;
      }
      buffer = new_buffer;
      capacity = new_capacity;
    }
    buffer[size] = c;
    buffer[size + 1] = '\0';
    ++size;
  } while (c != '\n');
  return buffer;
}

struct Stack *parsed(struct Stack *stack, struct Stack *queue, char *str) {
  if (stack == NULL || queue == NULL || str == NULL) {
    return NULL;
  }
  char *buff = NULL;
  
  bool found = false;
  bool stop = true;
  bool empty = false;
  
  size_t start = 0;
  size_t i = 0;
  do{
    if(priority(str[i]) == DIGIT) {
      if (str[i] == '\0') {
        if (operand(queue, str, start, i, &found, &stop) == NULL) {  // if operand fails
          return NULL;
        }
      } else {
        if (found == false) {
          start = i;
        }
        found = true;
        stop = false;
      }
    } else if (str[i] == '(') {
      if (operand(queue, str, start, i, &found, &stop) == NULL) {  // if operand fails
        return NULL;
      }
      if (push_char(stack, str[i]) == NULL) {  // if push_char fails
        return NULL;
      }
      
    } else if (str[i] == ')') {
      if (operand(queue, str, start, i, &found, &stop) == NULL) {  // if operand fails
        return NULL;
      }
      if (pop(stack, &buff) == NULL) {  // if pop fails
        return NULL;
      }
      if (operand(queue, str, start, i, &found, &stop) == NULL) {  // if operand fails
        return NULL;
      }
      while(buff[0] != '(') {
        
        if (push_char(queue, buff[0]) == NULL) {  // if push_char fails
          return NULL;
        }
        if (pop(stack, &buff) == NULL) {  // if pop fails
          return NULL;
        }
      }
    } else {
      if (isEmpty(stack, &empty) == NULL) {  // if isEmpty fails
        return NULL;
      }
      if (operand(queue, str, start, i, &found, &stop) == NULL) {  // if operand fails
        return NULL;
      }
      if (!empty) {
        while ((priority(str[i])) <= priority(stack->buffer_[stack->top_]->str_[0])) {
          
          if (pop(stack, &buff) == NULL) {  // if pop fails
            return NULL;
          }
          if (push_char(queue, buff[0]) == NULL) {  // if push_char fails
            return NULL;
          }
          if (isEmpty(stack, &empty) == NULL) {  // if isEmpty fails
            return NULL;
          }
          if (empty) {  // exit loop if stack empty
            break;
          }
        }
      }
      if (push_char(stack, str[i]) == NULL) {  // if push_char fails
        return NULL;
      }
    }
    i++;
  } while (str[i - 1] != '\0');
  
  if (isEmpty(stack, &empty) == NULL) {  // if isEmpty fails
    return NULL;
  }
  while (!empty) {
    if (pop(stack, &buff) == NULL) {  // if pop fails
      return NULL;
    }
    if (push_char(queue, buff[0]) == NULL) {  // if push_char fails
      return NULL;
    }
    if (isEmpty(stack, &empty) == NULL) {  // if isEmpty fails
      return NULL;
    }
  }
  return queue;
}

struct Stack *reversed(struct Stack *queue, struct Stack *revers) {
  if (queue == NULL || revers == NULL) {
    return NULL;
  }
  char *buff = NULL;
  bool empty = false;
  
  if (isEmpty(queue, &empty) == NULL) {  // if isEmpty fails
    return NULL;
  }
  while (!empty) {
    if (pop(queue, &buff) == NULL) {  // if pop fails
      return NULL;
    }
    if (push(revers, buff) == NULL) {  // if push fails
      return NULL;
    }
    if (isEmpty(queue, &empty) == NULL) {  // if isEmpty fails
      return NULL;
    }
  }
  return revers;
}

struct Stack* evaluated(struct Stack *revers, struct Stack *eval) {
  if (revers == NULL || eval == NULL) {
    return NULL;
  }
  char *buff = NULL;
  size_t size = 0;
  
  double num = 0;
  char* check =NULL;
  char* op1 = NULL;
  char* op2 = NULL;
  char* tok = NULL;
  
  while (revers->size_ >= 1) {
    num = strtod(revers->buffer_[revers->top_]->str_, &check);
    if (revers->buffer_[revers->top_]->str_ == check) {
      if (pop(revers, &tok) == NULL) {  // if pop fails
        return NULL;
      }
      if (pop(eval, &op2) == NULL) {  // if pop fails
        return NULL;
      }
      if (pop(eval, &op1) == NULL) {  // if pop fails
        return NULL;
      }
      if (doMath(revers, tok, op1, op2, &num) == NULL) {  // if doMath fails
        return NULL;
      }
      size = countDigits(num, 2);
      char* handler = malloc((size + 1) * sizeof(char));
      if (handler == NULL) {  // if malloc fails
        return NULL;
      }
      sprintf(handler,"%.2f", num);
      if (push(eval, handler) == NULL) {  // if push fails
        return NULL;
      }
      free(handler);
    } else {
      if (pop(revers, &buff) == NULL) {  // if pop fails
        return NULL;
      }
      if (push(eval, buff) == NULL) {  // if push fails
        return NULL;
      }
    }
  }
  return eval;
}

int main(int argc, char *argv[]) {

  struct Stack *stack = create_stack();
  struct Stack *queue = create_stack();
  struct Stack *revers = create_stack();
  struct Stack *eval = create_stack();
  
  bool is_eof = false;
  char *str = NULL;
  
  str = getString(str ,&is_eof);
  if (str == NULL) {  // if getString fails
    printf("[error]");
    goto free;
  }
  if (parsed(stack, queue, str) == NULL) {  // if parsed fails
    printf("[error]");
    goto free;
  }
  if (reversed(queue, revers) == NULL) {  // if reversed fails
    printf("[error]");
    goto free;
  }
  if (evaluated(revers, eval) == NULL) {  // if evaluated fails
    printf("[error]");
    goto free;
  }
  
  char *buff = NULL;
  if (pop(eval, &buff) == NULL) {  // if pop fails
    printf("[error]");
    goto free;
  }
  printf("%s\n", buff);
  
free:
  free_stack(eval);
  free_stack(revers);
  free_stack(queue);
  free_stack(stack);
  free(str);
  return 0;
}

