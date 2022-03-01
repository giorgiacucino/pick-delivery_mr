;; Auto-generated. Do not edit!


(when (boundp 'pick_delivery::c_to_s)
  (if (not (find-package "PICK_DELIVERY"))
    (make-package "PICK_DELIVERY"))
  (shadow 'c_to_s (find-package "PICK_DELIVERY")))
(unless (find-package "PICK_DELIVERY::C_TO_S")
  (make-package "PICK_DELIVERY::C_TO_S"))

(in-package "ROS")
;;//! \htmlinclude c_to_s.msg.html


(defclass pick_delivery::c_to_s
  :super ros::object
  :slots (_resp _idrob ))

(defmethod pick_delivery::c_to_s
  (:init
   (&key
    ((:resp __resp) 0)
    ((:idrob __idrob) 0)
    )
   (send-super :init)
   (setq _resp (round __resp))
   (setq _idrob (round __idrob))
   self)
  (:resp
   (&optional __resp)
   (if __resp (setq _resp __resp)) _resp)
  (:idrob
   (&optional __idrob)
   (if __idrob (setq _idrob __idrob)) _idrob)
  (:serialization-length
   ()
   (+
    ;; int64 _resp
    8
    ;; int64 _idrob
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int64 _resp
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _resp (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _resp) (= (length (_resp . bv)) 2)) ;; bignum
              (write-long (ash (elt (_resp . bv) 0) 0) s)
              (write-long (ash (elt (_resp . bv) 1) -1) s))
             ((and (class _resp) (= (length (_resp . bv)) 1)) ;; big1
              (write-long (elt (_resp . bv) 0) s)
              (write-long (if (>= _resp 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _resp s)(write-long (if (>= _resp 0) 0 #xffffffff) s)))
     ;; int64 _idrob
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _idrob (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _idrob) (= (length (_idrob . bv)) 2)) ;; bignum
              (write-long (ash (elt (_idrob . bv) 0) 0) s)
              (write-long (ash (elt (_idrob . bv) 1) -1) s))
             ((and (class _idrob) (= (length (_idrob . bv)) 1)) ;; big1
              (write-long (elt (_idrob . bv) 0) s)
              (write-long (if (>= _idrob 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _idrob s)(write-long (if (>= _idrob 0) 0 #xffffffff) s)))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int64 _resp
#+(or :alpha :irix6 :x86_64)
      (setf _resp (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _resp (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; int64 _idrob
#+(or :alpha :irix6 :x86_64)
      (setf _idrob (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _idrob (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;;
   self)
  )

(setf (get pick_delivery::c_to_s :md5sum-) "e9e9ddd2ccd7a9c901bb09b4fb8d85be")
(setf (get pick_delivery::c_to_s :datatype-) "pick_delivery/c_to_s")
(setf (get pick_delivery::c_to_s :definition-)
      "int64 resp
int64 idrob
")



(provide :pick_delivery/c_to_s "e9e9ddd2ccd7a9c901bb09b4fb8d85be")


