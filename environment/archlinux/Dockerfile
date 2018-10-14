FROM archlinux/base
RUN mkdir /tmp/build

COPY build.d/common /tmp/build/

COPY build.d/stage_1_setup_environment /tmp/build/
RUN chmod +x /tmp/build/stage_1_setup_environment && /tmp/build/stage_1_setup_environment

COPY build.d/stage_2_install_packages /tmp/build/
RUN chmod +x /tmp/build/stage_2_install_packages && sudo -u developer /tmp/build/stage_2_install_packages

COPY build.d/stage_optional_build_and_install_v8 /tmp/build/
RUN chmod +x /tmp/build/stage_optional_build_and_install_v8 && sudo -u developer /tmp/build/stage_optional_build_and_install_v8

RUN rm -rf /tmp/*
