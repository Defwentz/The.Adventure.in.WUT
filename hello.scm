#lang scheme

(define instructions "h/help for instructions.\n")
(define unrecognized-action "don't know what you mean.\n")
(define running-into-wall "sorry, some weird forces are controlling you, which makes you couldn't go there.\n")

; map: ((area0 n s w e description items) (area1 ...) ...)
(define forest-map
  '((Entrance Forest0 w w w "You are standing at the entrance to some weird forest.
Somehow you don't have any choice except going in there..." '())
    (Forest0 Forest0 w Entrance Forest0 "Everything looks the same." '())))

(define map-helper ; helping to get location's information
  (lambda (map loc f)
    (cond
      [(null? map) '()]
      [(eq? (caar map) loc)
       (f (cdar map))]
      [else (map-helper (cdr map) loc f)])))
(define (forestmap-helper loc f)
  (map-helper forest-map loc f))
; location's information: (n s w e description items)
(define (loc-north info)
    (car info))
(define (loc-south info)
    (cadr info))
(define (loc-west info)
    (caddr info))
(define (loc-east info)
    (cadddr info))
(define (loc-des info)
    (string-append (car (cddddr info))
                   "\n"))
(define print-location
  (lambda (location)
    (display (string-append (symbol->string location)
                            "\n"
                            (forestmap-helper location loc-des)))))

;player: (health inventory)
(define player '(100 '()))

(define location 'Entrance)
(define (print-curloc) ; current location
  (print-location location))
(define move
  (lambda (dir)
    (cond
      [(eq? (forestmap-helper location dir) 'w)
       (display running-into-wall)]
      [else (begin
              [set! location (forestmap-helper location dir)]
              [print-curloc])])))

(define expression-to-action
  (lambda (exp)
    (cond
      [(or (string=? "h" (car exp)) (string=? "help" (car exp)))
       (display instructions)]
      [(or (string=? "n" (car exp)) (string=? "north" (car exp)))
       (move loc-north)]
      [(or (string=? "s" (car exp)) (string=? "south" (car exp)))
       (move loc-south)]
      [(or (string=? "w" (car exp)) (string=? "west" (car exp)))
       (move loc-west)]
      [(or (string=? "e" (car exp)) (string=? "east" (car exp)))
       (move loc-east)]
      [(string=? "look" (car exp))
       (print-curloc)]
      [(string=? "quit" (car exp))
       (exit)]
      [else
       (display unrecognized-action)])))

(define main-loop
  (lambda (input)
    (begin
      [cond
        [(not (string? input))
         (display "input should be string.\n")]
        [else
         (expression-to-action (string-split input))]]
      [main-loop (read-line)])))

(define forest-game
  (lambda ()
    (begin
      (print-curloc)
      [main-loop (read-line)])))