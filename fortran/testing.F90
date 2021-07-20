!Test

module test
contains
    subroutine print(string)
        character(*) :: string
        write(*, *) string
    end subroutine print


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
end module test


