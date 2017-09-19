#!/usr/bin/python3.5
import sys, os, re, datetime, argparse

version_pro_file = "../src/LazyFarmer.pro"
installation_date = str(datetime.datetime.today()).split()[0]

def export_details(path):
    with open(path) as origin_file:
        for line in origin_file:
            line = re.findall(r'VERSION(\s*)=(\s*)(.*)', line)
            if line:
                return line[0][2]

    return "0.0.0.0"

def update_version(path, out_path, version):
    with open(path, "r") as origin_file:
        contents = origin_file.read().format(VERSION = version, DATE=installation_date)
        with open(out_path, "w") as dest_file:
            dest_file.write(contents)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Specify output directory paths")
    parser.add_argument('--out-pwd', help="Output directory")
    args = parser.parse_args()
    print(args.out_pwd)

    version = export_details(version_pro_file)
    update_version("../install/config/config.xml",  "{}/config.xml".format(args.out_pwd), version)
    update_version("../install/package/pl.wololo.lazyfarmer/meta/package.xml", "{}/package.xml".format(args.out_pwd), version)
