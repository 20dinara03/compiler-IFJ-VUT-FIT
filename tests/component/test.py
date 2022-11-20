import os
import subprocess
import difflib
from typing import Optional

FILE: str = "setup.env"


class TestException(Exception):
    pass


def read_file(path) -> list:
    with open(path) as f:
        return f.read().splitlines()


def run(_bin: str, scenario_dir: str, *, env: Optional[dict] = None) -> str:
    """
    Run the binary with the given scenario

    Raises:
        TestException: If the binary returns a non-zero exit code
    """
    sp = subprocess.Popen(
        [_bin, scenario_dir],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        env=env,
    )
    out, err = sp.communicate()
    if err:
        raise TestException(err)
    else:
        return out.decode()


def get_scenarios(scenarios_path: str) -> iter:
    for scenario in os.listdir(scenarios_path):
        yield scenario


def make_diff(actual: str, expected: str) -> str:
    PATH: str = "/tmp/expected.txt"

    def store_tmp_file(content: str):
        with open(PATH, "w") as f:
            f.write(content)

    def remove_tmp_file():
        os.remove(PATH)

    store_tmp_file(actual)
    diff = difflib.unified_diff(
        read_file(PATH),
        read_file(expected),
        fromfile="actual",
        tofile="expected",
    )
    remove_tmp_file()
    return "\n".join(diff)


def print_test_result(scenario: str, diff: str):
    def print_diff():
        for line in diff.splitlines():
            if line.startswith("+"):
                print(f"\033[92m{line}\033[0m")
            elif line.startswith("-"):
                print(f"\033[91m{line}\033[0m")
            else:
                print(line)

    class Color:
        GREEN = "\033[92m"
        RED = "\033[91m"
        END = "\033[0m"
    if diff:
        print(f"{Color.RED}Test failed for {scenario}{Color.END}")
        print_diff()
    else:
        print(f"{Color.GREEN}Test passed for {scenario}{Color.END}")


def main():
    config: dict = {}
    if os.path.isfile(FILE):
        for var in read_file(FILE):
            _var, _val = var.split("=")
            config[_var] = _val

    binary = f"{config.get('bin_dir')}/{config.get('bin')}"
    if not os.path.isfile(binary):
        raise TestException(f"Binary {binary} does not exist")

    for _scenario_dir in get_scenarios(config["scenarios"]):
        scenario_dir = f"{config['scenarios']}/{_scenario_dir}"
        for scenario in os.listdir(scenario_dir):
            test = f"{scenario_dir}/{scenario}"
            if not ("input.txt" in os.listdir(test) and
                    "expected.txt" in os.listdir(test)):
                raise TestException(f"Missing input or expected file in {test}")
            output = run(binary, f"{test}/input.txt", env={"IFJ2022_COMPONENT": _scenario_dir})
            diff = make_diff(output, f"{test}/expected.txt")
            print_test_result(test, diff)


if __name__ == "__main__":
    main()
