# Dbus service file
servicedir = $(DBUS_SERVICES_DIR)
service_in_files = org.openprinting.Backend.GCP.service.in
service_DATA = $(service_in_files:.service.in=.service)

$(service_DATA): $(service_in_files) Makefile
	@sed -e "s|\@cpdb_backend_dir\@|$(CPDB_BACKEND_DIR)|" $<> $@

info_backenddir = $(INFO_DIR)
info_backend_DATA = org.openprinting.Backend.GCP

clean-local:
	rm -rf $(service_DATA)
