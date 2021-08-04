!Test

module testing
    public

    integer, parameter :: testjeee = 1
    doubleprecision, parameter :: haha = 1.2
    character(*), parameter :: TEST = "yese"

    character(10) :: hahastring = "haha"

    character(4) :: listje(4) = ["haha", "jepe", "yese", "hihi"]

contains
    subroutine print(string)
        character(*) :: string
        write(*, *) testjeee
    end subroutine print


    subroutine ndarray(a)
        integer, intent(out) :: a(2,3)
        a(1,1) = 1
        a(1,2) = 2
        a(1,3) = 3
        a(2,1) = 4
        a(2,2) = 5
        a(2,3) = 6
!        a(3,1) = 7
!        a(3,2) = 8
!        a(3,3) = 9
    end subroutine ndarray


    subroutine optional(a, b, res)
        integer, intent(in) :: a
        integer, optional, intent(in) :: b
        integer, intent(out) :: res
        if (present(b)) then
            res = a + b
        else
            res = a
        endif
    end subroutine optional

    function writeTest()
        write(*, *)  "Hello World!"
    end function writeTest


    subroutine add(a, b, result)
        integer, intent(in) :: a
        integer, intent(out) :: b
        integer, intent(out) :: result
        result = a + b
    end subroutine add

    integer function add_old(a, b)
        integer :: a
        integer :: b
        add_old = a + b
    end function add_old
end module testing


