(define c531
  (lambda (input)
    (begin
      (cond
        ((not (string? input))
         (display "should be string.\n"))
        ((string=? input "code")
         (display "good job.\n"))
        (else
         (display "nah...\n")))
      (c531 (read-line)))))

(define string-to-token 0)

(define main
  (lambda ()
    (begin
      (display "Welcome!\nGuess the code:\n")
      (c531 (read-line)))))
;(main)