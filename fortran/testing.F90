!Test

module testing
    public

    integer, parameter :: testjeee = 1
    doubleprecision, parameter :: haha = 1.2
    character(*), parameter :: TEST = "yese"

contains
    subroutine print(string)
        character(*) :: string
        write(*, *) testjeee
    end subroutine print

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


